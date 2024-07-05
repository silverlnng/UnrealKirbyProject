#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
<<<<<<< Updated upstream

    UFUNCTION(BlueprintCallable)
    void OnHit(float Damage);  // 적이 공격을 받을 때 호출되는 함수

=======
    void OnHit(int32 Damage);  // 적이 공격을 받을 때 호출되는 함수
>>>>>>> Stashed changes

private:
    void CheckFireCondition();
    void Fire();
    void RotateToPlayer(float DeltaTime);
    void Die();  // 적이 죽을 때 호출되는 함수
};
