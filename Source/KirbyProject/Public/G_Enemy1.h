#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "Components/InputComponent.h"
#include "EnemyState.h" // E열거형 공유파일
#include "G_Enemy1.generated.h"


UCLASS()
class KIRBYPROJECT_API AG_Enemy1 : public ACharacter
{
	GENERATED_BODY()

	
public:	
	AG_Enemy1();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	APawn* PlayerPawn;
	
	void SetState(EEnemyState NewState);
	void PlayAnimMontage(UAnimMontage* MontageToPlay);
	
	void InitAnimation();
	void IdleAnimNotify();
	void AttackAnimNotify();
	void DeathAnimNotify();
	void Idle(); // 기본 상태
	void CheckAttackCondition();
	void Attack(float DeltaTime); // 적이 공격할 때
	void Die();  // 적이 죽을 때

	void UpdateAnimation(float DeltaTime);  // 상태에 따른 애니메이션 업데이트

public:	
	virtual void Tick(float DeltaTime) override;

	// 적의 체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "G_Enemy1")
	float Health = 1.0f;  

	// 코인 클래스 선언
	UPROPERTY(EditDefaultsOnly, Category = "Coin")
    TSubclassOf<class AActor> CoinClass;  

	UFUNCTION(BlueprintCallable)
    void Damage(float DamageAmount); // 데미지 받는 부분

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

	//UPROPERTY(EditDefaultsOnly, Category = "G_Enemy1")
	//float moveSpeed	 = 50.0f; 

	// 별 VFX (맞을 때)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	class UParticleSystem* StarVFX; 
	// 연기 VFX (죽을 때 펑)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	class UParticleSystem* SmokeVFX;

private:
	// 공격 범위
    UPROPERTY(EditDefaultsOnly, Category = "G_Enemy1")
    float attackRange = 600.0f;

	FTimerHandle BlinkTimerHandle;
	void StartBlinkEffect();
	void StopBlinkEffect();
};
