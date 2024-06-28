#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwordEnemyFSM.generated.h"

// ����� ���� ����
UENUM(BlueprintType)
enum class EEnemyState : unit8 
{
	Idle,
	Move,
	Attack,
	Damage,
	Die,
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class KIRBYPROJECT_API USwordEnemyFSM : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	USwordEnemyFSM();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=FSM)
	EEnemyState mState = EEnemyState::Idle;

	// ��� ����
	void IdleState();
	//�̵� ����
	void MoveState();
	// ���� ����
	void AttackState();
	// �ǰ� ����
	void DamageState();
	// ���� ����
	void DieState();

	// ��� �ð�
	UPROPERTY(EditDefaultsOnly, Category=FSM)
	float idleDelayTime = 2;

	// ��� �ð�
	float currentTime = 0;

	// Ÿ��
	UPROPERTY(VisibleAnywhere, Category=FSM)
	class AKirbyProjectCharacter* target;

	// ���� ����
	UPROPERTY()
	class ASwordEnemy* me;

	// ���� ����
	UPROPERTY(EditAnywhere, Category = FSM)
	float attackRange = 150.0f;
};

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