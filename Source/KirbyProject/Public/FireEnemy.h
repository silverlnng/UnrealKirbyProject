#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/Timelinecomponent.h"
#include "FireEnemy.generated.h"

UCLASS()
class KIRBYPROJECT_API AFireEnemy : public AActor
{
    GENERATED_BODY()

private:
    UPROPERTY(VisibleAnywhere)
    USceneComponent* Root;

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

    UFUNCTION(BlueprintCallable)
    void OnHit(float Damage);  // 적이 공격을 받을 때 호출되는 함수

private:
    void CheckFireCondition();
    void Fire();
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
};
