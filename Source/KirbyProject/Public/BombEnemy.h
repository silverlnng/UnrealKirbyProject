#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "EnemyState.h" // E열거형 공유파일
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
    TSubclassOf<class AActor> CoinClass;  // 코인 클래스 선언

    //UPROPERTY(VisibleAnywhere, Category = Collision)
    //class UCapsuleComponent* DetectionCapsule;

    FTimerHandle BombTimerHandle;

    APawn* PlayerPawn;

    float Health;  // Enemy 체력

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
    void Idle(); // 기본 상태
    void Die();  // 적이 죽을 때

    void UpdateAnimation(float DeltaTime);  // 상태에 따른 애니메이션 업데이트

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable)
    void OnHit(float Damage);  // Enemy가 공격을 받을 때 호출되는 함수

    UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, 
                        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, 
                        const FHitResult & SweepResult);

    void ApplySuckingForce();

    // 기본 애니메이션 몽타주
	UPROPERTY(EditDefaultsOnly, Category="Animation") 
	UAnimMontage* IdleAnimMontage;

	// 공격 애니메이션 몽타주
	UPROPERTY(EditDefaultsOnly, Category="Animation") 
	UAnimMontage* AttackAnimMontage;

	// 죽음 애니메이션 몽타주
	UPROPERTY(EditDefaultsOnly, Category="Animation") 
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BombEnemy")
	EEnemyState CurrentState;

    // 별 VFX (맞을 때)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	class UParticleSystem* StarVFX; 
	// 연기 VFX (죽을 때 펑)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	class UParticleSystem* SmokeVFX;

private:
    void CheckBombCondition();
    void Bomb();
    void RotateToPlayer(float DeltaTime);

    // 맞을 때 하얗게 깜박이는 효과
    FTimerHandle BlinkTimerHandle;
    void StartBlinkEffect();
    void StopBlinkEffect();
};
