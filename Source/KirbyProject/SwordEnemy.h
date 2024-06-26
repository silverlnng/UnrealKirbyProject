#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwordEnemy.generated.h"

UCLASS()
class KIRBYPROJECT_API ASwordEnemy : public AActor
{
	GENERATED_BODY()
	
private:
    UPROPERTY(EditDefaultsOnly, Category = "SwordEnemy")
    float ChaseRange;

    UPROPERTY(EditDefaultsOnly, Category = "SwordEnemy")
    float AttackRange;

    UPROPERTY(EditDefaultsOnly, Category = "SwordEnemy")
    float AttackInterval;

    UPROPERTY(EditDefaultsOnly, Category = "SwordEnemy")
    class UAnimMontage* AttackMontage;

    FTimerHandle AttackTimerHandle;

    APawn* PlayerPawn;

    bool bIsPlayerInRange(float Range);

public:	
	ASwordEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
    void CheckChaseConditoin();
    void CheckAttackCondition();
    void Attack();
};
