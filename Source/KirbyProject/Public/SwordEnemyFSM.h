#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwordEnemyFSM.generated.h"

// 사용할 상태 정의
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
	// 상태 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=FSM)
	EEnemyState mState = EEnemyState::Idle;

	// 대기 상태
	void IdleState();
	//이동 상태
	void MoveState();
	// 공격 상태
	void AttackState();
	// 피격 상태
	void DamageState();
	// 죽음 상태
	void DieState();

	// 대기 시간
	UPROPERTY(EditDefaultsOnly, Category=FSM)
	float idleDelayTime = 2;

	// 경과 시간
	float currentTime = 0;

	// 타깃
	UPROPERTY(VisibleAnywhere, Category=FSM)
	class AKirbyProjectCharacter* target;

	// 소유 액터
	UPROPERTY()
	class ASwordEnemy* me;

	// 공격 범위
	UPROPERTY(EditAnywhere, Category = FSM)
	float attackRange = 150.0f;
};

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