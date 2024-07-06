#include "SwordEnemyFSM.h"
#include "KirbyProjectCharacter.h"
#include "SwordEnemy.h" 
#include "Kismet/GameplayStatics.h"
//#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Math/UnrealMathUtility.h"
//#include "Animation/AnimInstance.h"
//#include "Engine/World.h"


USwordEnemyFSM::USwordEnemyFSM()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USwordEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	//월드에서 AKirbyProjectCharacter 타깃 찾아오기
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AKirbyProjectCharacter::StaticClass());
	// AKirbyProjectCharacter타입으로 캐스팅
	target = Cast<AKirbyProjectCharacter>(actor);
	// 소유 객체 가져오기
	me = Cast<ASwordEnemy>(GetOwner());
	

	if (me)
	{
		// 적의 기본 이동 속도를 설정
		me->GetCharacterMovement()->MaxWalkSpeed = moveSpeed;
	}
}

void USwordEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckPlayerDistance();

	switch (mState) 
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState(DeltaTime);
		break;
	case EEnemyState::Attack:
		AttackState(3.0);
		break;
	/*case EEnemyState::Damage:
		DamageState();
		break;*/
	case EEnemyState::Die:
		DieState();
		break;
	}

}
// 플레이어 거리 측정
void USwordEnemyFSM::CheckPlayerDistance()
{
	if (target)
	{
		float distance = FVector::Dist(me->GetActorLocation(), target->GetActorLocation());

		if (distance <= attackRange)
		{
			//mState = EEnemyState::Attack;
		}
		else if (distance <= chaseRange)
		{
			mState = EEnemyState::Move;
		}
		else
		{
			mState = EEnemyState::Idle;
		}
	}
}

// 대기 상태
void USwordEnemyFSM::IdleState()
{
	// 시간이 흘렀으니까
	currentTime += GetWorld()->DeltaTimeSeconds;

	// 만약 경과 시간이 대기 시간 초과했다면
	if (currentTime > idleDelayTime)
	{
		// 이동 상태로 전환
		mState = EEnemyState::Move;
		// 경과 시간 초기화
		currentTime = 0;
	}
}

// 이동 상태
void USwordEnemyFSM::MoveState(float DeltaTime)
{
	if(target)
	{
		// 타깃 목적지가 필요
		FVector destination = target->GetActorLocation();

		// 방향 필요
		FVector dir = destination - me->GetActorLocation();
		dir.Z = 0;  // 방향 벡터의 Z 축을 0으로 설정하여 수평 이동만 하도록 함\



		
		if (!dir.IsNearlyZero())
		{
			// 방향으로 이동하고 싶음
			me->AddMovementInput(dir.GetSafeNormal());

			// 적의 방향을 플레이어를 향하도록 회전
			FRotator targetRotation = dir.Rotation();
			FRotator currentRotation = me->GetActorRotation();

			targetRotation.Yaw += -90.0f;

			FRotator newRotation = FMath::RInterpTo(currentRotation, targetRotation, DeltaTime, 5.0f);  // 회전 속도를 조정할 수 있습니다.
			me->SetActorRotation(newRotation);
		}
	}
}

void USwordEnemyFSM::AttackState(float Damage)
{
	Health -= Damage;
	if (Health <= 0)
	{
		DieState();
	}
}

void USwordEnemyFSM::DieState()
{
	if (CoinClass)
	{
		FVector SpawnLocation = me->GetActorLocation();
		FRotator SpawnRotation = me->GetActorRotation();
		GetWorld()->SpawnActor<AActor>(CoinClass, SpawnLocation, SpawnRotation);
	}

	//Destroy();  // 적을 제거
	if (me)
	{
		// 적의 모든 컴포넌트를 비활성화
		me->SetActorHiddenInGame(true);
		me->SetActorEnableCollision(false);
		me->SetActorTickEnabled(false);

		// 모든 메쉬 컴포넌트 비활성화
		TArray<UActorComponent*> Components;
		me->GetComponents(Components);
		for (UActorComponent* Component : Components)
		{
			Component->Deactivate();
		}
	}
}

//// 공격 상태
//void USwordEnemyFSM::AttackState() 
//{
//	if (target && me->GetDistanceTo(target) <= attackRange)
//	{
//		if (me->GetMesh() && AttackMontage)
//		{
//			UAnimInstance* AnimInstance = me->GetMesh()->GetAnimInstance();
//			if (AnimInstance && !AnimInstance->Montage_IsPlaying(AttackMontage))
//			{
//				AnimInstance->Montage_Play(AttackMontage);
//
//				// 데미지 이벤트를 애니메이션 몽타주의 특정 시점에서 트리거합니다.
//				FTimerHandle TimerHandle;
//				GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
//					{
//						if (target && me->GetDistanceTo(target) <= attackRange)
//						{
//							// 데미지를 줍니다 (데미지 값과 로직은 필요에 따라 수정)
//							target->TakeDamage(10.0f, FDamageEvent(), nullptr, me);
//						}
//					}, 0.5f, false);  // 0.5초 후에 데미지를 줍니다 (애니메이션에 맞춰 타이밍 조정)
//			}
//		}
//	}
//}

//// 피격 상태
//void USwordEnemyFSM::DamageState() {}
//
//// 죽음 상태
//void USwordEnemyFSM::DieState() {}