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


AG_Enemy1::AG_Enemy1()
{
	PrimaryActorTick.bCanEverTick = true;

	attackRange = 2000.0f; // 플레이어가 다가와야 하는 거리
	Health = 3.0f;  // 초기 체력 설정

	CurrentState = EEnemyState::Idle;  // 초기 상태를 Idle로 설정
}

void AG_Enemy1::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	InitAnimation();
}

//void AG_Enemy1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	if (auto* EnhancedInputcomponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
//	{
//		EnhancedInputcomponent->BindAction(AnimNotifyAction, ETriggerEvent::Started, this, &AG_Enemy1::AnimNotifyPressed);
//	}
//}


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
			PlayAnimMontage(IdleAnimMontage);
			break;
		case EEnemyState::Attack:
			PlayAnimMontage(AttackAnimMontage);
			Attack(3.0f);
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
	// 기본 애니메이션 재생
	//if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(AttackAnimMontage))
	//{
	//	PlayAnimMontage(AttackAnimMontage);
	//}
	PlayAnimMontage(AttackAnimMontage);
}

void AG_Enemy1::CheckAttackCondition()
{
	if (PlayerPawn)
	{
		float DistanceToPlayer = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());

		if (DistanceToPlayer <= attackRange)
		{
			// 공격 시작
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
		// 타깃 목적지가 필요
		FVector destination = PlayerPawn->GetActorLocation();

		// 방향 필요
		FVector dir = destination - GetActorLocation();
		dir.Z = 0;  // 방향 벡터의 Z 축을 0으로 설정하여 수평 이동만 하도록 함


		if (!dir.IsNearlyZero())
		{
			// 방향으로 이동하고 싶음
			FVector MoveAction(dir.GetSafeNormal());

			// 적의 방향을 플레이어를 향하도록 회전
			FRotator targetRotation = dir.Rotation();
			FRotator currentRotation = GetActorRotation();

			targetRotation.Yaw += -90.0f;

			FRotator newRotation = FMath::RInterpTo(currentRotation, targetRotation, DeltaTime, 5.0f);  // 회전 속도 조정할 수 있음
			SetActorRotation(newRotation);
		}
	}
}

void AG_Enemy1::Damage(float damage)
{
	Health -= damage;
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

	//Destroy();  // 적을 제거
	
	// 적의 모든 컴포넌트를 비활성화
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);

	// 모든 메쉬 컴포넌트 비활성화
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
		// Idle 상태에서 실행할 로직
		break;
	case EEnemyState::Attack:
		// Attack 상태에서 실행할 로직
		Attack(DeltaTime);
		break;
	case EEnemyState::Dead:
		// Dead 상태에서 실행할 로직
		Die();
		break;
	default:
		break;
	}
}