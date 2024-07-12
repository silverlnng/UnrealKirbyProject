#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "G_Enemy2.generated.h"

UCLASS()
class KIRBYPROJECT_API AG_Enemy2 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AG_Enemy2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
