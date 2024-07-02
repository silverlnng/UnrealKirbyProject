#include "SwordEnemyFSM.h"
#include "KirbyProject/KirbyProjectCharacter.h"
#include "SwordEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Math/UnrealMathUtility.h"


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
		AttackState();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
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
			mState = EEnemyState::Attack;
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
		dir.Z = 0;  // 방향 벡터의 Z 축을 0으로 설정하여 수평 이동만 하도록 함

		
		if (!dir.IsNearlyZero())
		{
			// 방향으로 이동하고 싶음
			me->AddMovementInput(dir.GetSafeNormal());

			// 적의 방향을 플레이어를 향하도록 회전
			FRotator targetRotation = dir.Rotation();
			FRotator currentRotation = me->GetActorRotation();
			FRotator newRotation = FMath::RInterpTo(currentRotation, targetRotation, DeltaTime, 5.0f);  // 회전 속도를 조정할 수 있습니다.
			me->SetActorRotation(newRotation);
		}
	}
}

// 공격 상태
void USwordEnemyFSM::AttackState() 
{
	if (target && me->GetDistanceTo(target) <= attackRange)
	{
		// 애니메이션을 재생하거나 공격 로직을 추가
		// 예: me->PlayAttackMontage();
	}
}

// 피격 상태
void USwordEnemyFSM::DamageState() {}

// 죽음 상태
void USwordEnemyFSM::DieState() {}