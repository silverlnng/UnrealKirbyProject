#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = CollisionComponent;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 500.0f;
	ProjectileMovement->MaxSpeed = 500.0f;

	ProjectileMovement->ProjectileGravityScale = 0.0f; // �߷� ���� �� �޵���
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	SpiralRadius = 50.0f; // ������ �̵� �ݰ�
	SpiralSpeed = 0.2f; // ������ �̵� �ӵ�
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	GameModeBase = UGameplayStatics::GetGameMode(GetWorld());
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ���� �ð�
	//float CurrentTime = 0.0f; 
	CurrentTime += UGameplayStatics::GetTimeSeconds(GetWorld());

	float ElapsedTime = CurrentTime - DeltaTime;
	
	// ������ �̵� ���� ���
	float NewX = SpiralRadius * FMath::Sin(SpiralSpeed * ElapsedTime);
	float NewZ = SpiralRadius * FMath::Cos(SpiralSpeed * ElapsedTime);

	UE_LOG(LogTemp, Display, TEXT("X: %f"), NewX);
	UE_LOG(LogTemp, Display, TEXT("Z: %f"), NewZ);

	FVector ForwardVector = GetActorForwardVector() * ProjectileMovement->InitialSpeed * DeltaTime;
	FVector NewLocation = ForwardVector + FVector(NewX, 0.0f, NewZ);

	// Projectile �̵�
	ProjectileMesh->SetRelativeLocation(NewLocation);

	//CollisionComponent->K2_SetRelativeLocation(NewLocation, false, FHitResult * OutSweepHitResult = nullptr, ETeleportType Teleport = ETeleportType::None);
}

void AProjectile::SpawnTrailEffect()
{
	if (TrailVFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TrailVFX, GetActorLocation());
	}
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
//	if (ExplosionVFX)
//	{
		if (GameModeBase) {
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionVFX, GetActorLocation());
			// �浹 �� Projectile �ı�
			Destroy();
		}
//	}
}