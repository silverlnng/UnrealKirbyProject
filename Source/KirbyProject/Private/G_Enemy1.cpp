// DeerDoe
#include "G_Enemy1.h"
#include "KirbyProjectCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "IdleAnimNotify.h"
#include "AttackAnimNotify.h"
#include "DeathAnimNotify.h"
#include "AIController.h"
#include "Materials/MaterialInstanceDynamic.h"


AG_Enemy1::AG_Enemy1()
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentState = EEnemyState::Idle;  // �ʱ� ���¸� Idle�� ����
}

void AG_Enemy1::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	InitAnimation();
}


void AG_Enemy1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckAttackCondition();
	UpdateAnimation(DeltaTime);
}

void AG_Enemy1::SetState(EEnemyState NewState)
{
	if (CurrentState != NewState)
	{
		CurrentState = NewState;
		switch (NewState)
		{
		case EEnemyState::Idle:
			Idle();
			break;
		case EEnemyState::Attack:
			PlayAnimMontage(AttackAnimMontage);
			break;
		case EEnemyState::Dead:
			PlayAnimMontage(DeathAnimMontage);
			Die();
			break;
		default:
			break;
		}
	}
}

void AG_Enemy1::Idle()
{
	PlayAnimMontage(IdleAnimMontage);
}

void AG_Enemy1::CheckAttackCondition()
{
	if (PlayerPawn)
	{
		float DistanceToPlayer = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());

		UE_LOG(LogTemp, Warning, TEXT("Distance to Player: %f"), DistanceToPlayer); // ����� �޽��� �߰�

		if (DistanceToPlayer <= attackRange)
		{
			UE_LOG(LogTemp, Warning, TEXT("Switching to Attack State")); // ����� �޽��� �߰�
			// ���� ����
			SetState(EEnemyState::Attack);
		}
		else
		{
			SetState(EEnemyState::Idle);
		}
	}
}

void AG_Enemy1::Attack(float DeltaTime)
{
	if (PlayerPawn)
	{
		// Ÿ�� �������� �ʿ�
		FVector destination = PlayerPawn->GetActorLocation();

		// ���� �ʿ�
		FVector dir = destination - GetActorLocation();
		dir.Z = 0;  // ���� ������ Z ���� 0���� �����Ͽ� ���� �̵��� �ϵ��� ��


		if (!dir.IsNearlyZero())
		{
			// ���� ������ �÷��̾ ���ϵ��� ȸ��
			FRotator targetRotation = dir.Rotation();
			FRotator currentRotation = GetActorRotation();

			targetRotation.Yaw += -90.0f;

			FRotator newRotation = FMath::RInterpTo(currentRotation, targetRotation, DeltaTime, 5.0f);  // ȸ�� �ӵ� ������ �� ����
			SetActorRotation(newRotation);

			// AI ��Ʈ�ѷ��� ���� �÷��̾ ���� �̵�
			AAIController* AIController = Cast<AAIController>(GetController());
			if (AIController)
			{
				// �޸� �� ���� ȿ��
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SmokeVFX, GetActorLocation());
				AIController->MoveToActor(PlayerPawn, 5.0f); // 5.0f�� ��� ���� ����
			}
		}
	}
}

void AG_Enemy1::Damage(float DamageAmount)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), StarVFX, GetActorLocation());
	StartBlinkEffect(); // ������ ���� �� �����̱� ����
	Health -= DamageAmount;
	if (Health <= 0)
	{
		Die();
	}
}

void AG_Enemy1::Die()
{
	SetState(EEnemyState::Dead);

	if (CoinClass)
	{
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();
		GetWorld()->SpawnActor<AActor>(CoinClass, SpawnLocation, SpawnRotation);
	}

	// ���� �ùķ��̼� Ȱ��ȭ �� �� ����
	GetMesh()->SetSimulatePhysics(true);
	FVector LaunchDirection = FVector(FMath::RandRange(-1.0f, 1.0f), FMath::RandRange(-1.0f, 1.0f), 1.0f);
	LaunchDirection.Normalize();
	FVector LaunchForce = LaunchDirection * 1000.0f; // ���� ũ�� ����
	GetMesh()->AddImpulse(LaunchForce);

	// ���� �ð� �� �ı�
	SetLifeSpan(1.0f); // 1�� �Ŀ� �ı�
	
	// ���� �� �� VFX
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SmokeVFX, GetActorLocation());

	// ���� ��� ������Ʈ�� ��Ȱ��ȭ
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);

	//Destroy();  // ���� ����

	// ��� �޽� ������Ʈ ��Ȱ��ȭ
	TArray<UActorComponent*> Components;
	GetComponents(Components);
	for (UActorComponent* Component : Components)
	{
		Component->Deactivate();
	}	
}


void AG_Enemy1::InitAnimation()
{
	if (IdleAnimMontage)
	{
		const auto NotifyEvents = IdleAnimMontage->Notifies;
		for (FAnimNotifyEvent EventNotify : NotifyEvents)
		{
			if (const auto IdleNotify = Cast<UIdleAnimNotify>(EventNotify.Notify))
			{
				IdleNotify->OnNotified.AddUObject(this, &AG_Enemy1::IdleAnimNotify);
			}
		}
	}
	if (AttackAnimMontage)
	{
		const auto NotifyEvents = AttackAnimMontage->Notifies;
		for (FAnimNotifyEvent EventNotify : NotifyEvents)
		{
			if (const auto AttackNotify = Cast<UAttackAnimNotify>(EventNotify.Notify))
			{
				AttackNotify->OnNotified.AddUObject(this, &AG_Enemy1::AttackAnimNotify);
			}
		}
	}
	if (DeathAnimMontage)
	{
		const auto NotifyEvents = DeathAnimMontage->Notifies;
		for (FAnimNotifyEvent EventNotify : NotifyEvents)
		{
			if (const auto DeathNotify = Cast<UDeathAnimNotify>(EventNotify.Notify))
			{
				DeathNotify->OnNotified.AddUObject(this, &AG_Enemy1::DeathAnimNotify);
			}
		}
	}
}

void AG_Enemy1::IdleAnimNotify()
{
	UE_LOG(LogTemp, Warning, TEXT("Idle Anim was finished"));
}

void AG_Enemy1::AttackAnimNotify()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack Anim was finished"));
}

void AG_Enemy1::DeathAnimNotify()
{
	UE_LOG(LogTemp, Warning, TEXT("Death Anim was finished"));
}


void AG_Enemy1::PlayAnimMontage(UAnimMontage* MontageToPlay)
{
	if (GetMesh() && MontageToPlay)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay);
	}
}

void AG_Enemy1::UpdateAnimation(float DeltaTime)
{
	switch (CurrentState)
	{
	case EEnemyState::Idle:
		// Idle ���¿��� ������ ����
		break;
	case EEnemyState::Attack:
		// Attack ���¿��� ������ ����
		Attack(DeltaTime);
		break;
	case EEnemyState::Dead:
		// Dead ���¿��� ������ ����
		Die();
		break;
	default:
		break;
	}
}

void AG_Enemy1::StartBlinkEffect()
{
	if (UMaterialInstanceDynamic* MatInstance = GetMesh()->CreateAndSetMaterialInstanceDynamic(0))
	{
		MatInstance->SetScalarParameterValue(FName("BlinkAmount"), 1.0f); // �����̱� ����
	}
	GetWorldTimerManager().SetTimer(BlinkTimerHandle, this, &AG_Enemy1::StopBlinkEffect, 2.0f, false); // 2�� �Ŀ� ������ ����
}

void AG_Enemy1::StopBlinkEffect()
{
	if (UMaterialInstanceDynamic* MatInstance = GetMesh()->CreateAndSetMaterialInstanceDynamic(0))
	{
		MatInstance->SetScalarParameterValue(FName("BlinkAmount"), 0.0f); // ������ ����
	}
}