#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Fireball.generated.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

UCLASS()
class KIRBYPROJECT_API AFireball : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFireball();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// �Լ� ����� �Է¿� ���ε��ϱ� ���� ȣ��
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ���Ǿ� �ݸ��� ������Ʈ�Դϴ�.
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComponent;

	// �߻�ü �̵� ������Ʈ�Դϴ�.
	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;

	// �߻�ü �߻縦 ó���ϴ� �Լ��Դϴ�.
	UFUNCTION()
	void Fire();

	// ī�޶� ��ġ�κ����� �ѱ� �������Դϴ�.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	/*UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* FireballMesh;

	class UProjectileMovementComponent* FireballComponent;*/
};
