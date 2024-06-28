#include "SwordEnemyFSM.h"
#include "KirbyProject/KirbyProjectCharacter.h"
#include "SwordEnemy.h"
#include <Kismet/GameplayStatics.h>


USwordEnemyFSM::USwordEnemyFSM()
{
	PrimaryActorTick.bCanEverTick = true;

}

void USwordEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	//월드에서 AKirbyProjectCharacter 타깃 찾아오기
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AKirbyProjectCharacter::StaticClass());
	// AKirbyProjectCharacter타입으로 캐스팅
	target = Cast< AKirbyProjectCharacter>(actor);
	// 소유 객체 가져오기
	me = Cast< ASwordEnemy>(GetOwner());
	
}

void USwordEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mState) 
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
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

// 대기 상태
void USwordEnemyFSM::IdleState(){}

// 이동 상태
void USwordEnemyFSM::MoveState() 
{
	// 타깃 목적지가 필요
	FVector destination = target->GetActorLocation();

	// 방향 필요
	FVector dir = destination = me->GetActorLocation();

	// 방향으로 이동하고 싶음
	me->AddMovementInput(dir.GetSafeNormal());
}

// 공격 상태
void USwordEnemyFSM::AttackState() {}

// 피격 상태
void USwordEnemyFSM::DamageState() {}

// 죽음 상태
void USwordEnemyFSM::DieState() {}