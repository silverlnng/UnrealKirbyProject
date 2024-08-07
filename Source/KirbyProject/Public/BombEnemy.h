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

    float BombInterval;

    UPROPERTY(EditDefaultsOnly, Category = "BombEnemy")
    TSubclassOf<class ABombProjectile> BombProjectileClass;

    UPROPERTY(EditDefaultsOnly, Category = "BombEnemy")
    TSubclassOf<class AActor> CoinClass;  // 코인 클래스 선언


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
    void ThrowBomb();
    void StartThrowingBomb();
    void RotateToPlayer(float DeltaTime);

    AActor* HoldBomb; // 생성된 폭탄 저장하는 멤버 변수

    UPROPERTY(EditAnywhere)
    float BombThrowDelay;  // 폭탄 던지기까지 대기 시간

    float DeadDelay; // 죽는 애니메이션 재생될 시간

    FTimerHandle TimerHandle;

    void KnockBack();
};
