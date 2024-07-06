// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BombEnemy.generated.h"

UCLASS()
class KIRBYPROJECT_API ABombEnemy : public AActor
{
	GENERATED_BODY()
private:
    UPROPERTY(VisibleAnywhere)
    USceneComponent* Root;

    UPROPERTY(EditDefaultsOnly, Category = "BombEnemy")
    USkeletalMeshComponent* BombMesh;

    UPROPERTY(EditDefaultsOnly, Category = "BombEnemy")
    float BombRange;

    UPROPERTY(EditDefaultsOnly, Category = "BombEnemy")
    float BombInterval;

    UPROPERTY(EditDefaultsOnly, Category = "BombEnemy")
    TSubclassOf<class ABombProjectile> BombProjectileClass;

    UPROPERTY(EditDefaultsOnly, Category = "BombEnemy")
    TSubclassOf<class AActor> CoinClass;  // ���� Ŭ���� ����

    UPROPERTY(VisibleAnywhere, Category = Collision)
    class UCapsuleComponent* DetectionCapsule;

    FTimerHandle BombTimerHandle;

    APawn* PlayerPawn;

    float Health;  // Enemy ü��

public:	
	// Sets default values for this actor's properties
	ABombEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable)
    void OnHit(float Damage);  // Enemy�� ������ ���� �� ȣ��Ǵ� �Լ�

    UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, 
                        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, 
                        const FHitResult & SweepResult);

private:
    void CheckBombCondition();
    void Bomb();
    void RotateToPlayer(float DeltaTime);
    void Die();  // ���� ���� �� ȣ��Ǵ� �Լ�
};
