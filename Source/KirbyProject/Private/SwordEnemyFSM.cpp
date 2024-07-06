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

	//���忡�� AKirbyProjectCharacter Ÿ�� ã�ƿ���
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AKirbyProjectCharacter::StaticClass());
	// AKirbyProjectCharacterŸ������ ĳ����
	target = Cast<AKirbyProjectCharacter>(actor);
	// ���� ��ü ��������
	me = Cast<ASwordEnemy>(GetOwner());
	

	if (me)
	{
		// ���� �⺻ �̵� �ӵ��� ����
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
// �÷��̾� �Ÿ� ����
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
		dir.Z = 0;  // ���� ������ Z ���� 0���� �����Ͽ� ���� �̵��� �ϵ��� ��\



		
		if (!dir.IsNearlyZero())
		{
			// �������� �̵��ϰ� ����
			me->AddMovementInput(dir.GetSafeNormal());

			// ���� ������ �÷��̾ ���ϵ��� ȸ��
			FRotator targetRotation = dir.Rotation();
			FRotator currentRotation = me->GetActorRotation();

			targetRotation.Yaw += -90.0f;

			FRotator newRotation = FMath::RInterpTo(currentRotation, targetRotation, DeltaTime, 5.0f);  // ȸ�� �ӵ��� ������ �� �ֽ��ϴ�.
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

	//Destroy();  // ���� ����
	if (me)
	{
		// ���� ��� ������Ʈ�� ��Ȱ��ȭ
		me->SetActorHiddenInGame(true);
		me->SetActorEnableCollision(false);
		me->SetActorTickEnabled(false);

		// ��� �޽� ������Ʈ ��Ȱ��ȭ
		TArray<UActorComponent*> Components;
		me->GetComponents(Components);
		for (UActorComponent* Component : Components)
		{
			Component->Deactivate();
		}
	}
}

//// ���� ����
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
//				// ������ �̺�Ʈ�� �ִϸ��̼� ��Ÿ���� Ư�� �������� Ʈ�����մϴ�.
//				FTimerHandle TimerHandle;
//				GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
//					{
//						if (target && me->GetDistanceTo(target) <= attackRange)
//						{
//							// �������� �ݴϴ� (������ ���� ������ �ʿ信 ���� ����)
//							target->TakeDamage(10.0f, FDamageEvent(), nullptr, me);
//						}
//					}, 0.5f, false);  // 0.5�� �Ŀ� �������� �ݴϴ� (�ִϸ��̼ǿ� ���� Ÿ�̹� ����)
//			}
//		}
//	}
//}

//// �ǰ� ����
//void USwordEnemyFSM::DamageState() {}
//
//// ���� ����
//void USwordEnemyFSM::DieState() {}