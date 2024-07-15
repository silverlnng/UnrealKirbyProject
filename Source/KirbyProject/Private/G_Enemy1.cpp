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

	CurrentState = EEnemyState::Idle;  // 초기 상태를 Idle로 설정
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

		UE_LOG(LogTemp, Warning, TEXT("Distance to Player: %f"), DistanceToPlayer); // 디버그 메시지 추가

		if (DistanceToPlayer <= attackRange)
		{
			UE_LOG(LogTemp, Warning, TEXT("Switching to Attack State")); // 디버그 메시지 추가
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
			// 적의 방향을 플레이어를 향하도록 회전
			FRotator targetRotation = dir.Rotation();
			FRotator currentRotation = GetActorRotation();

			targetRotation.Yaw += -90.0f;

			FRotator newRotation = FMath::RInterpTo(currentRotation, targetRotation, DeltaTime, 5.0f);  // 회전 속도 조정할 수 있음
			SetActorRotation(newRotation);

			// AI 컨트롤러를 통해 플레이어를 향해 이동
			AAIController* AIController = Cast<AAIController>(GetController());
			if (AIController)
			{
				// 달릴 때 뭉게 효과
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SmokeVFX, GetActorLocation());
				AIController->MoveToActor(PlayerPawn, 5.0f); // 5.0f는 허용 오차 범위
			}
		}
	}
}

void AG_Enemy1::Damage(float DamageAmount)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), StarVFX, GetActorLocation());
	StartBlinkEffect(); // 데미지 입을 때 깜박이기 시작
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

	// 물리 시뮬레이션 활성화 및 힘 적용
	GetMesh()->SetSimulatePhysics(true);
	FVector LaunchDirection = FVector(FMath::RandRange(-1.0f, 1.0f), FMath::RandRange(-1.0f, 1.0f), 1.0f);
	LaunchDirection.Normalize();
	FVector LaunchForce = LaunchDirection * 1000.0f; // 힘의 크기 조절
	GetMesh()->AddImpulse(LaunchForce);

	// 일정 시간 후 파괴
	SetLifeSpan(1.0f); // 1초 후에 파괴
	
	// 죽을 때 펑 VFX
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SmokeVFX, GetActorLocation());

	// 적의 모든 컴포넌트를 비활성화
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);

	//Destroy();  // 적을 제거

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

void AG_Enemy1::StartBlinkEffect()
{
	if (UMaterialInstanceDynamic* MatInstance = GetMesh()->CreateAndSetMaterialInstanceDynamic(0))
	{
		MatInstance->SetScalarParameterValue(FName("BlinkAmount"), 1.0f); // 깜박이기 시작
	}
	GetWorldTimerManager().SetTimer(BlinkTimerHandle, this, &AG_Enemy1::StopBlinkEffect, 2.0f, false); // 2초 후에 깜박임 종료
}

void AG_Enemy1::StopBlinkEffect()
{
	if (UMaterialInstanceDynamic* MatInstance = GetMesh()->CreateAndSetMaterialInstanceDynamic(0))
	{
		MatInstance->SetScalarParameterValue(FName("BlinkAmount"), 0.0f); // 깜박임 종료
	}
}