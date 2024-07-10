#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = CollisionComponent;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 3000.0f;
	ProjectileMovement->MaxSpeed = 3000.0f;

	SpiralRadius = 20.0f; // ������ �̵� �ݰ�
	SpiralSpeed = 10.0f; // ������ �̵� �ӵ�
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// �ð�����
	CurrentTime = UGameplayStatics::GetTimeSeconds(GetWorld());
	
	// ������ �̵� ���� ���
	float NewY = SpiralRadius * FMath::Sin(SpiralSpeed * CurrentTime);
	float NewZ = SpiralRadius * FMath::Cos(SpiralSpeed * CurrentTime);

	FVector NewLocation = FVector(0.0f, NewY, NewZ);

	// Projectile �̵�
	USceneComponent::K2_SetRelativeLocation
}

