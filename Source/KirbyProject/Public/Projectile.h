#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h" 
#include "Components/SphereComponent.h" 
#include "Projectile.generated.h"

UCLASS()
class KIRBYPROJECT_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	USphereComponent* CollisionComponent;

	//// 회전 속도
 //   UPROPERTY(EditAnywhere, Category = "Movement")
 //   float RotationSpeed;

 //   // Sine 함수 활용한 TimeAccumulator
 //   float TimeAccumulator;

	// 원형 궤적의 반지름
    UPROPERTY(EditAnywhere, Category = "Movement")
    float CircleRadius;

    // 원형 궤적의 속도
    UPROPERTY(EditAnywhere, Category = "Movement")
    float CircleSpeed;

    // 시간 누적 변수
    float TimeAccumulator;

    // Projectile 시작 위치
    FVector StartLocation;

};
