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
    TSubclassOf<class AActor> CoinClass;  // ���� Ŭ���� ����

    FTimerHandle FireTimerHandle;

    APawn* PlayerPawn;

    float Health;  // ���� ü��

public:	
	AFireEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable)
    void OnHit(float Damage);  // ���� ������ ���� �� ȣ��Ǵ� �Լ�

private:
    void CheckFireCondition();
    void Fire();
    //void RotateToPlayer(float DeltaTime);
    void Die();  // ���� ���� �� ȣ��Ǵ� �Լ�

    // Ÿ�Ӷ��� �ִϸ��̼�
    UFUNCTION()
    void UpdateScale(float ScaleValue);

    // Ÿ�Ӷ��� ������Ʈ �� curve
    UPROPERTY()
    UTimelineComponent* FireTimeline;

    UPROPERTY(EditAnywhere, Category = "Animation")
    UCurveFloat* ScaleCurve;
};
