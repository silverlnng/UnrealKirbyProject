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

	//���忡�� AKirbyProjectCharacter Ÿ�� ã�ƿ���
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AKirbyProjectCharacter::StaticClass());
	// AKirbyProjectCharacterŸ������ ĳ����
	target = Cast< AKirbyProjectCharacter>(actor);
	// ���� ��ü ��������
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

// ��� ����
void USwordEnemyFSM::IdleState(){}

// �̵� ����
void USwordEnemyFSM::MoveState() 
{
	// Ÿ�� �������� �ʿ�
	FVector destination = target->GetActorLocation();

	// ���� �ʿ�
	FVector dir = destination = me->GetActorLocation();

	// �������� �̵��ϰ� ����
	me->AddMovementInput(dir.GetSafeNormal());
}

// ���� ����
void USwordEnemyFSM::AttackState() {}

// �ǰ� ����
void USwordEnemyFSM::DamageState() {}

// ���� ����
void USwordEnemyFSM::DieState() {}