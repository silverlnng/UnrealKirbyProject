#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h" 
#include "Components/SphereComponent.h" 
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
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

	// 날아가는 VFX
	UPROPERTY(EditAnywhere)
	class UParticleSystem* TrailVFX;

	// 이펙트 함수
	void SpawnTrailEffect();

	// 충돌 이벤트 핸들러
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// 충돌 VFX
	UPROPERTY(EditAnywhere)
	class UParticleSystem* ExplosionVFX;

	// 나이아가라 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* NiagaraComponent;
	void EnableNiagara();
	float NiagaraInterval;
	bool bCanSpawnNiagara;

	// 회전 속도
    UPROPERTY(EditAnywhere, Category = "Movement")
    float SpiralSpeed; // 나선형 이동 속도
	float SpiralRadius; // 나선형 이동 반경

	float CurrentTime = 0.0f;
	// 시작 위치
	FVector StartLocation;

private:
	// 게임 모드를 저장할 변수
	AGameModeBase* GameModeBase;
	// 타이머
	FTimerHandle TimerHandle;
};
