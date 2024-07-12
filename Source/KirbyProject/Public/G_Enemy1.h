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

	// ���� Ŭ���� ����
	UPROPERTY(EditDefaultsOnly, Category = "Coin")
    TSubclassOf<class AActor> CoinClass;  

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

	// ���� ü��
	UPROPERTY(EditDefaultsOnly, Category = "G_Enemy1")
	float Health;  

	// ���� ����
    UPROPERTY(EditDefaultsOnly, Category = "G_Enemy1")
    float attackRange = 150.0f;

	UPROPERTY(EditDefaultsOnly, Category = "G_Enemy1")
	float moveSpeed	 = 100.0f;

private:

	UPROPERTY()
	APawn* PlayerPawn;

	//// ���� �Է� �׼�
	//UPROPERTY(EditAnywhere, Category="Input") 
	//UInputAction* AttackAction;

	//// �ִϸ��̼� ��Ƽ���� �׼�
	//UPROPERTY(EditDefaultsOnly, Category="Input") 
	//UInputAction* AnimNotifyAction;  


	void SetState(EEnemyState NewState);
	void PlayAnimMontage(UAnimMontage* MontageToPlay);
	
	void InitAnimation();
	void IdleAnimNotify();
	void AttackAnimNotify();
	void DeathAnimNotify();
	//void AnimNotifyPressed();
	void Idle(); // �⺻ ����
	void CheckAttackCondition();
	void Attack(float DeltaTime); // ���� ������ ��
	void Damage(float damage); // ���� ���� ��
	void Die();  // ���� ���� ��

	void UpdateAnimation(float DeltaTime);  // ���¿� ���� �ִϸ��̼� ������Ʈ
};
