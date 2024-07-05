#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "SwordEnemyFSM.generated.h"

// ����� ���� ����
UENUM(BlueprintType)
enum class EEnemyState : uint8 
{
	Idle UMETA(DisplayName = "Idle"),
	Move UMETA(DisplayName = "Move"),
	Attack UMETA(DisplayName = "Attack"),
	Damage UMETA(DisplayName = "Damage"),
	Die UMETA(DisplayName = "Die")
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class KIRBYPROJECT_API USwordEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:
	USwordEnemyFSM();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable)
    void AttackState(float Damage);  // ���� ������ ���� �� ȣ��Ǵ� �Լ�

private:
	// ��� ����
	void IdleState();
	//�̵� ����
	void MoveState(float DeltaTime);

	//// �ǰ� ����
	//void DamageState();
	// ���� ���� �� ȣ��Ǵ� �Լ�
	void DieState();  

	void CheckPlayerDistance();

public:
	// ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	EEnemyState mState = EEnemyState::Idle;
	

	// ��� �ð�
	UPROPERTY(EditDefaultsOnly, Category = FSM)
	float idleDelayTime = 2.0f;


	UPROPERTY(EditDefaultsOnly, Category = FSM)
    float chaseRange = 400.0f;
	
	// ���� ����
    UPROPERTY(EditDefaultsOnly, Category = FSM)
    float attackRange = 150.0f;

	UPROPERTY(EditDefaultsOnly, Category = FSM)
	float moveSpeed	 = 100.0f;


	// ��� �ð�
	float currentTime = 0.0f;

	// Ÿ��
	UPROPERTY(VisibleAnywhere, Category = FSM)
	class AKirbyProjectCharacter* target;

	// ���� ����
	UPROPERTY()
	class ASwordEnemy* me;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
    UAnimMontage* AttackMontage;  // ���� �ִϸ��̼� ��Ÿ��

	UPROPERTY(EditDefaultsOnly, Category = FSM)
    TSubclassOf<class AActor> CoinClass;  // ���� Ŭ���� ����

	float Health = 3.0f;  // ���� ü��
};