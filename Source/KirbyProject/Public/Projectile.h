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

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
    UStaticMeshComponent* ProjectileMesh;

	// ���ư��� VFX
	UPROPERTY(EditAnywhere)
	class UParticleSystem* TrailVFX;

	// ����Ʈ �Լ�
	void SpawnTrailEffect();

	// �浹 �̺�Ʈ �ڵ鷯
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// �浹 VFX
	UPROPERTY(EditAnywhere)
	class UParticleSystem* ExplosionVFX;

	// ȸ�� �ӵ�
    UPROPERTY(EditAnywhere, Category = "Movement")
    float SpiralSpeed; // ������ �̵� �ӵ�
	float SpiralRadius; // ������ �̵� �ݰ�

	float CurrentTime = 0.0f;
	// ���� ��ġ
	FVector StartLocation;

private:
	// ���� ��带 ������ ����
	AGameModeBase* GameModeBase;
};
