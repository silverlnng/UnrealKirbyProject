#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "Components/InputComponent.h"
#include "EnemyState.h" // E������ ��������
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
	void Idle(); // �⺻ ����
	void CheckAttackCondition();
	void Attack(float DeltaTime); // ���� ������ ��
	void Die();  // ���� ���� ��

	void UpdateAnimation(float DeltaTime);  // ���¿� ���� �ִϸ��̼� ������Ʈ

public:	
	virtual void Tick(float DeltaTime) override;

	// ���� ü��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "G_Enemy1")
	float Health = 1.0f;  

	// ���� Ŭ���� ����
	UPROPERTY(EditDefaultsOnly, Category = "Coin")
    TSubclassOf<class AActor> CoinClass;  

	UFUNCTION(BlueprintCallable)
    void Damage(float DamageAmount); // ������ �޴� �κ�

	// �⺻ �ִϸ��̼� ��Ÿ��
	UPROPERTY(EditDefaultsOnly, Category="Animation") 
	UAnimMontage* IdleAnimMontage;

	// ���� �ִϸ��̼� ��Ÿ��
	UPROPERTY(EditDefaultsOnly, Category="Animation") 
	UAnimMontage* AttackAnimMontage;

	// ���� �ִϸ��̼� ��Ÿ��
	UPROPERTY(EditDefaultsOnly, Category="Animation") 
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "G_Enemy1")
	EEnemyState CurrentState;

	//UPROPERTY(EditDefaultsOnly, Category = "G_Enemy1")
	//float moveSpeed	 = 50.0f; 

	// �� VFX (���� ��)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	class UParticleSystem* StarVFX; 
	// ���� VFX (���� �� ��)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	class UParticleSystem* SmokeVFX;

private:
	// ���� ����
    UPROPERTY(EditDefaultsOnly, Category = "G_Enemy1")
    float attackRange = 600.0f;

	FTimerHandle BlinkTimerHandle;
	void StartBlinkEffect();
	void StopBlinkEffect();
};
