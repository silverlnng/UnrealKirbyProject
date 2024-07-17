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

    //UFUNCTION(BlueprintCallable)
    //void OnHit(float Damage);  // ���� ������ ���� �� ȣ��Ǵ� �Լ�

    // ���� VFX
	UPROPERTY(EditAnywhere)
	class UParticleSystem* ExplosionVFX; 

private:
    void CheckFireCondition();
    void Fire();
    // ������ ���� �Լ�
    void StartSequence();
    // ������ ���� �Լ�
    void StopSequence();
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

    // �߻�ü ���� ī��Ʈ
    int32 ProjectileCount;

    // Ÿ�̸� �ڵ鷯
    FTimerHandle ProjectileTimerHandle;
    FTimerHandle SequenceTimerHandle;
};
