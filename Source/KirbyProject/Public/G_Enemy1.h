#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "Components/InputComponent.h"
#include "G_Enemy1.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
    Idle,
    Attack,
    Dead
};

UCLASS()
class KIRBYPROJECT_API AG_Enemy1 : public ACharacter
{
	GENERATED_BODY()

	
public:	
	AG_Enemy1();

protected:
	virtual void BeginPlay() override;
	//virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:	
	virtual void Tick(float DeltaTime) override;

	// 코인 클래스 선언
	UPROPERTY(EditDefaultsOnly, Category = "Coin")
    TSubclassOf<class AActor> CoinClass;  

	// 기본 애니메이션 몽타주
	UPROPERTY(EditDefaultsOnly, Category="Animation") 
	UAnimMontage* IdleAnimMontage;

	// 공격 애니메이션 몽타주
	UPROPERTY(EditDefaultsOnly, Category="Animation") 
	UAnimMontage* AttackAnimMontage;

	// 죽음 애니메이션 몽타주
	UPROPERTY(EditDefaultsOnly, Category="Animation") 
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "G_Enemy1")
	EEnemyState CurrentState;

	// 적의 체력
	UPROPERTY(EditDefaultsOnly, Category = "G_Enemy1")
	float Health;  

	// 공격 범위
    UPROPERTY(EditDefaultsOnly, Category = "G_Enemy1")
    float attackRange = 150.0f;

	UPROPERTY(EditDefaultsOnly, Category = "G_Enemy1")
	float moveSpeed	 = 100.0f;

private:

	UPROPERTY()
	APawn* PlayerPawn;

	//// 공격 입력 액션
	//UPROPERTY(EditAnywhere, Category="Input") 
	//UInputAction* AttackAction;

	//// 애니메이션 노티파이 액션
	//UPROPERTY(EditDefaultsOnly, Category="Input") 
	//UInputAction* AnimNotifyAction;  


	void SetState(EEnemyState NewState);
	void PlayAnimMontage(UAnimMontage* MontageToPlay);
	
	void InitAnimation();
	void IdleAnimNotify();
	void AttackAnimNotify();
	void DeathAnimNotify();
	//void AnimNotifyPressed();
	void Idle(); // 기본 상태
	void CheckAttackCondition();
	void Attack(float DeltaTime); // 적이 공격할 때
	void Damage(float damage); // 적이 맞을 때
	void Die();  // 적이 죽을 때

	void UpdateAnimation(float DeltaTime);  // 상태에 따른 애니메이션 업데이트
};
