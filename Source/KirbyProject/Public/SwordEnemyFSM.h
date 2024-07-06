#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "SwordEnemyFSM.generated.h"

// 사용할 상태 정의
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
    void AttackState(float Damage);  // 적이 공격을 받을 때 호출되는 함수

private:
	// 대기 상태
	void IdleState();
	//이동 상태
	void MoveState(float DeltaTime);

	//// 피격 상태
	//void DamageState();
	// 적이 죽을 때 호출되는 함수
	void DieState();  

	void CheckPlayerDistance();

public:
	// 상태 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	EEnemyState mState = EEnemyState::Idle;
	

	// 대기 시간
	UPROPERTY(EditDefaultsOnly, Category = FSM)
	float idleDelayTime = 2.0f;


	UPROPERTY(EditDefaultsOnly, Category = FSM)
    float chaseRange = 400.0f;
	
	// 공격 범위
    UPROPERTY(EditDefaultsOnly, Category = FSM)
    float attackRange = 150.0f;

	UPROPERTY(EditDefaultsOnly, Category = FSM)
	float moveSpeed	 = 100.0f;


	// 경과 시간
	float currentTime = 0.0f;

	// 타깃
	UPROPERTY(VisibleAnywhere, Category = FSM)
	class AKirbyProjectCharacter* target;

	// 소유 액터
	UPROPERTY()
	class ASwordEnemy* me;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
    UAnimMontage* AttackMontage;  // 공격 애니메이션 몽타주

	UPROPERTY(EditDefaultsOnly, Category = FSM)
    TSubclassOf<class AActor> CoinClass;  // 코인 클래스 선언

	float Health = 3.0f;  // 적의 체력
};