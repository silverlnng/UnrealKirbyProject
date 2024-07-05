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
<<<<<<< Updated upstream

    UFUNCTION(BlueprintCallable)
    void OnHit(float Damage);  // ���� ������ ���� �� ȣ��Ǵ� �Լ�

=======
    void OnHit(int32 Damage);  // ���� ������ ���� �� ȣ��Ǵ� �Լ�
>>>>>>> Stashed changes

private:
    void CheckFireCondition();
    void Fire();
    void RotateToPlayer(float DeltaTime);
    void Die();  // ���� ���� �� ȣ��Ǵ� �Լ�
};
