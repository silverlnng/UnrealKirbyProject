#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/Timelinecomponent.h"
#include "Components/BoxComponent.h"
#include "FireEnemy.generated.h"

UCLASS()
class KIRBYPROJECT_API AFireEnemy : public AActor
{
    GENERATED_BODY()

private:
    UPROPERTY(EditDefaultsOnly)
    UBoxComponent* Root;

    UPROPERTY(EditDefaultsOnly, Category = "FireEnemy")
    USkeletalMeshComponent* FireMesh;

    UPROPERTY(EditDefaultsOnly, Category = "FireEnemy")
    float FireRange;

    UPROPERTY(EditDefaultsOnly, Category = "FireEnemy")
    float FireInterval;

    UPROPERTY(EditDefaultsOnly, Category = "FireEnemy")
    TSubclassOf<class AProjectile> FireProjectileClass;


    UPROPERTY(EditDefaultsOnly, Category = "FireEnemy")
    TSubclassOf<class AActor> CoinClass;  // 코인 클래스 선언

    FTimerHandle FireTimerHandle;

    APawn* PlayerPawn;

    float Health;  // 적의 체력

public:	
	AFireEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

    //UFUNCTION(BlueprintCallable)
    //void OnHit(float Damage);  // 적이 공격을 받을 때 호출되는 함수

    // 폭발 VFX
	UPROPERTY(EditAnywhere)
	class UParticleSystem* ExplosionVFX; 

private:
    void CheckFireCondition();
    void Fire();
    // 시퀀스 시작 함수
    void StartSequence();
    // 시퀀스 멈춤 함수
    void StopSequence();
    //void RotateToPlayer(float DeltaTime);
    void Die();  // 적이 죽을 때 호출되는 함수

    // 타임라인 애니메이션
    UFUNCTION()
    void UpdateScale(float ScaleValue);

    // 타임라인 컴포넌트 및 curve
    UPROPERTY()
    UTimelineComponent* FireTimeline;

    UPROPERTY(EditAnywhere, Category = "Animation")
    UCurveFloat* ScaleCurve;

    // 발사체 생성 카운트
    int32 ProjectileCount;

    // 타이머 핸들러
    FTimerHandle ProjectileTimerHandle;
    FTimerHandle SequenceTimerHandle;
};
