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

	//���忡�� AKirbyProjectCharacter Ÿ�� ã�ƿ���
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AKirbyProjectCharacter::StaticClass());
	// AKirbyProjectCharacterŸ������ ĳ����
	target = Cast<AKirbyProjectCharacter>(actor);
	// ���� ��ü ��������
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
// �÷��̾� �Ÿ� ����
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

// ��� ����
void USwordEnemyFSM::IdleState()
{
	// �ð��� �귶���ϱ�
	currentTime += GetWorld()->DeltaTimeSeconds;

	// ���� ��� �ð��� ��� �ð� �ʰ��ߴٸ�
	if (currentTime > idleDelayTime)
	{
		// �̵� ���·� ��ȯ
		mState = EEnemyState::Move;
		// ��� �ð� �ʱ�ȭ
		currentTime = 0;
	}
}

// �̵� ����
void USwordEnemyFSM::MoveState(float DeltaTime)
{
	if(target)
	{
		// Ÿ�� �������� �ʿ�
		FVector destination = target->GetActorLocation();

		// ���� �ʿ�
		FVector dir = destination - me->GetActorLocation();
		dir.Z = 0;  // ���� ������ Z ���� 0���� �����Ͽ� ���� �̵��� �ϵ��� ��

		
		if (!dir.IsNearlyZero())
		{
			// �������� �̵��ϰ� ����
			me->AddMovementInput(dir.GetSafeNormal());

			// ���� ������ �÷��̾ ���ϵ��� ȸ��
			FRotator targetRotation = dir.Rotation();
			FRotator currentRotation = me->GetActorRotation();
			FRotator newRotation = FMath::RInterpTo(currentRotation, targetRotation, DeltaTime, 5.0f);  // ȸ�� �ӵ��� ������ �� �ֽ��ϴ�.
			me->SetActorRotation(newRotation);
		}
	}
}

// ���� ����
void USwordEnemyFSM::AttackState() 
{
	if (target && me->GetDistanceTo(target) <= attackRange)
	{
		// �ִϸ��̼��� ����ϰų� ���� ������ �߰�
		// ��: me->PlayAttackMontage();
	}
}

// �ǰ� ����
void USwordEnemyFSM::DamageState() {}

// ���� ����
void USwordEnemyFSM::DieState() {}