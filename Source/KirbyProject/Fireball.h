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

	// 함수 기능을 입력에 바인딩하기 위해 호출
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 스피어 콜리전 컴포넌트입니다.
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComponent;

	// 발사체 이동 컴포넌트입니다.
	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;

	// 발사체 발사를 처리하는 함수입니다.
	UFUNCTION()
	void Fire();

	// 카메라 위치로부터의 총구 오프셋입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	/*UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* FireballMesh;

	class UProjectileMovementComponent* FireballComponent;*/
};
