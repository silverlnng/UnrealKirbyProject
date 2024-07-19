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
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"


AG_Enemy1::AG_Enemy1()
{
	PrimaryActorTick.bCanEverTick = true;

	MoveSpeed = 70.0f;

	DeadDelay = 1.0f; // �״� �ִϸ��̼� ����� �ð�

	CurrentState = EEnemyState::Idle;  // �ʱ� ���¸� Idle�� ����

	// Niagara ������Ʈ �ʱ�ȭ
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);
	NiagaraComponent->SetAutoActivate(false); // �ʱ⿡�� ��Ȱ��ȭ ����

	NiagaraInterval = 0.3f;
	bCanSpawnNiagara = true;
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
			NiagaraComponent->SetAutoActivate(false);
			Idle();
			break;
		case EEnemyState::Attack:
			PlayAnimMontage(AttackAnimMontage);
			break;
		case EEnemyState::Dead:
			PlayAnimMontage(DeathAnimMontage);
			KnockBack(); // �˹�
			GetWorldTimerManager().SetTimer(TimerHandle, this, &AG_Enemy1::Die, DeadDelay, false); // �ִϸ��̼� ����ϰ� 2�� �� ����
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

		//UE_LOG(LogTemp, Warning, TEXT("Distance to Player: %f"), DistanceToPlayer); // ����� �޽��� �߰�

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

			FRotator newRotation = FMath::RInterpTo(currentRotation, targetRotation, DeltaTime, 100.0f);  // ȸ�� �ӵ� ������ �� ����
			SetActorRotation(newRotation);

			// AI ��Ʈ�ѷ��� ���� �÷��̾ ���� �̵�
			AAIController* AIController = Cast<AAIController>(GetController());
			if (AIController)
			{
				if (bCanSpawnNiagara)
				{
					// �޸� �� ���� ȿ�� ���̾ư���
					NiagaraComponent->Activate(true);

					bCanSpawnNiagara = false;
					GetWorldTimerManager().SetTimer(TimerHandle, this, &AG_Enemy1::EnableNiagara, NiagaraInterval, false); // �� �� �������� ����
				}
				// �̵� �ӵ� ����
				GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;

				AIController->MoveToActor(PlayerPawn, 1.0f); // 1.0f�� ��� ���� ����
			}
		}
	}
}

void AG_Enemy1::Damage(float DamageAmount)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), StarVFX, GetActorLocation());

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
	
	// ���� �� �� VFX
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SmokeVFX, GetActorLocation());

	Destroy();  // ���� ����
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
		NiagaraComponent->SetAutoActivate(false);
		Idle();
		break;
	case EEnemyState::Attack:
		// Attack ���¿��� ������ ����
		Attack(DeltaTime);
		break;
	case EEnemyState::Dead:
		KnockBack(); // �˹�
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AG_Enemy1::Die, DeadDelay, false); // �ִϸ��̼� ����ϰ� 2�� �� ����
		break;
	default:
		break;
	}
}

void AG_Enemy1::EnableNiagara()
{
	bCanSpawnNiagara = true;
}

void AG_Enemy1::KnockBack()
{
	FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), PlayerPawn->GetActorLocation());

	Direction.X = -1.5f;
	Direction.Y = 1.5f;
	Direction.Z = 1.5f;
	Direction.Normalize();

	FVector LaunchVelocity = Direction * 500.0f;


	if (ACharacter* Character = Cast<ACharacter>(this))
	{
		Character->LaunchCharacter(LaunchVelocity, true, true);
	}

}