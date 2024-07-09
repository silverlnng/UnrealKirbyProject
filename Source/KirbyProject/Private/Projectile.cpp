#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = CollisionComponent;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 100.0f;
	ProjectileMovement->MaxSpeed = 100.0f;

	//RotationSpeed = 360.0f; // 회전 속도 (도/초)
	//TimeAccumulator = 0.0f;
	CircleRadius = 100.0f; // 원형 궤적의 반지름
	CircleSpeed = 5.0f; // 원형 궤적의 속도
	TimeAccumulator = 0.0f;
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

	//// 회전
	//FRotator NewRotation = GetActorRotation();
	//NewRotation.Yaw += RotationSpeed * DeltaTime;
	//SetActorRotation(NewRotation);

	//// 나선형 이동
	//FVector NewLocation = GetActorLocation();
	//TimeAccumulator += DeltaTime;
	//float SpiralRadius = 0.5f; // 나선형 반지름 (고정값)
	//float SpiralFrequency = 1.0f; // 나선형 빈도 (고정값)
	//NewLocation.X += SpiralRadius * FMath::Cos(SpiralFrequency * TimeAccumulator);
	//NewLocation.Y += SpiralRadius * FMath::Sin(SpiralFrequency * TimeAccumulator);
	//SetActorLocation(NewLocation);

	// 시간 누적
	TimeAccumulator += DeltaTime * CircleSpeed;

	// 원형 궤적 계산
	FVector NewLocation = StartLocation;
	NewLocation.X += CircleRadius * FMath::Cos(TimeAccumulator);
	NewLocation.Y += CircleRadius * FMath::Sin(TimeAccumulator);

	// Projectile 이동
	SetActorLocation(NewLocation);
}

