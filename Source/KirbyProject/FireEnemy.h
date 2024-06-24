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
public:	
	AFireEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
