#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "EnemyState.h" // E������ ��������
#include "BombEnemy.generated.h"



UCLASS()
class KIRBYPROJECT_API ABombEnemy : public ACharacter
{
	GENERATED_BODY()
private:
    //UPROPERTY(EditDefaultsOnly, Category = "BombEnemy")
    //USkeletalMeshComponent* BombMesh;

    UPROPERTY(EditDefaultsOnly, Category = "BombEnemy")
    float BombRange;

    UPROPERTY(EditDefaultsOnly, Category = "BombEnemy")
    float BombInterval;

    UPROPERTY(EditDefaultsOnly, Category = "BombEnemy")
    TSubclassOf<class ABombProjectile> BombProjectileClass;

    UPROPERTY(EditDefaultsOnly, Category = "BombEnemy")
    TSubclassOf<class AActor> CoinClass;  // ���� Ŭ���� ����

    //UPROPERTY(VisibleAnywhere, Category = Collision)
    //class UCapsuleComponent* DetectionCapsule;

    FTimerHandle BombTimerHandle;

    APawn* PlayerPawn;

    float Health;  // Enemy ü��

public:	
	ABombEnemy();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    void SetState(EEnemyState NewState);
    void PlayAnimMontage(UAnimMontage* MontageToPlay);

    void InitAnimation();
    void IdleAnimNotify();
    void AttackAnimNotify();
    void DeathAnimNotify();
    void Idle(); // �⺻ ����
    void Die();  // ���� ���� ��

    void UpdateAnimation(float DeltaTime);  // ���¿� ���� �ִϸ��̼� ������Ʈ

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable)
    void OnHit(float Damage);  // Enemy�� ������ ���� �� ȣ��Ǵ� �Լ�

    UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, 
                        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, 
                        const FHitResult & SweepResult);

    void ApplySuckingForce();

    // �⺻ �ִϸ��̼� ��Ÿ��
	UPROPERTY(EditDefaultsOnly, Category="Animation") 
	UAnimMontage* IdleAnimMontage;

	// ���� �ִϸ��̼� ��Ÿ��
	UPROPERTY(EditDefaultsOnly, Category="Animation") 
	UAnimMontage* AttackAnimMontage;

	// ���� �ִϸ��̼� ��Ÿ��
	UPROPERTY(EditDefaultsOnly, Category="Animation") 
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BombEnemy")
	EEnemyState CurrentState;

    // �� VFX (���� ��)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	class UParticleSystem* StarVFX; 
	// ���� VFX (���� �� ��)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	class UParticleSystem* SmokeVFX;

private:
    void CheckBombCondition();
    void Bomb();
    void RotateToPlayer(float DeltaTime);

    // ���� �� �Ͼ�� �����̴� ȿ��
    FTimerHandle BlinkTimerHandle;
    void StartBlinkEffect();
    void StopBlinkEffect();
};
