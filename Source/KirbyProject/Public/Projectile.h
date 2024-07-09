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

	//// ȸ�� �ӵ�
 //   UPROPERTY(EditAnywhere, Category = "Movement")
 //   float RotationSpeed;

 //   // Sine �Լ� Ȱ���� TimeAccumulator
 //   float TimeAccumulator;

	// ���� ������ ������
    UPROPERTY(EditAnywhere, Category = "Movement")
    float CircleRadius;

    // ���� ������ �ӵ�
    UPROPERTY(EditAnywhere, Category = "Movement")
    float CircleSpeed;

    // �ð� ���� ����
    float TimeAccumulator;

    // Projectile ���� ��ġ
    FVector StartLocation;

};
