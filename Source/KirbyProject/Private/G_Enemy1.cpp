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

	DeadDelay = 1.0f; // 죽는 애니메이션 재생될 시간

	CurrentState = EEnemyState::Idle;  // 초기 상태를 Idle로 설정

	// Niagara 컴포넌트 초기화
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);
	NiagaraComponent->SetAutoActivate(false); // 초기에는 비활성화 상태

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
			KnockBack(); // 넉백
			GetWorldTimerManager().SetTimer(TimerHandle, this, &AG_Enemy1::Die, DeadDelay, false); // 애니메이션 재생하고 2초 뒤 죽음
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

		//UE_LOG(LogTemp, Warning, TEXT("Distance to Player: %f"), DistanceToPlayer); // 디버그 메시지 추가

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

			FRotator newRotation = FMath::RInterpTo(currentRotation, targetRotation, DeltaTime, 100.0f);  // 회전 속도 조정할 수 있음
			SetActorRotation(newRotation);

			// AI 컨트롤러를 통해 플레이어를 향해 이동
			AAIController* AIController = Cast<AAIController>(GetController());
			if (AIController)
			{
				if (bCanSpawnNiagara)
				{
					// 달릴 때 뭉게 효과 나이아가라
					NiagaraComponent->Activate(true);

					bCanSpawnNiagara = false;
					GetWorldTimerManager().SetTimer(TimerHandle, this, &AG_Enemy1::EnableNiagara, NiagaraInterval, false); // 몇 초 간격으로 생성
				}
				// 이동 속도 조절
				GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;

				AIController->MoveToActor(PlayerPawn, 1.0f); // 1.0f는 허용 오차 범위
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
	
	// 죽을 때 펑 VFX
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SmokeVFX, GetActorLocation());

	Destroy();  // 적을 제거
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
		// Attack 상태에서 실행할 로직
		Attack(DeltaTime);
		break;
	case EEnemyState::Dead:
		KnockBack(); // 넉백
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AG_Enemy1::Die, DeadDelay, false); // 애니메이션 재생하고 2초 뒤 죽음
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