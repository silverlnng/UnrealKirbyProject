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

	//RotationSpeed = 360.0f; // ȸ�� �ӵ� (��/��)
	//TimeAccumulator = 0.0f;
	CircleRadius = 100.0f; // ���� ������ ������
	CircleSpeed = 5.0f; // ���� ������ �ӵ�
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

	//// ȸ��
	//FRotator NewRotation = GetActorRotation();
	//NewRotation.Yaw += RotationSpeed * DeltaTime;
	//SetActorRotation(NewRotation);

	//// ������ �̵�
	//FVector NewLocation = GetActorLocation();
	//TimeAccumulator += DeltaTime;
	//float SpiralRadius = 0.5f; // ������ ������ (������)
	//float SpiralFrequency = 1.0f; // ������ �� (������)
	//NewLocation.X += SpiralRadius * FMath::Cos(SpiralFrequency * TimeAccumulator);
	//NewLocation.Y += SpiralRadius * FMath::Sin(SpiralFrequency * TimeAccumulator);
	//SetActorLocation(NewLocation);

	// �ð� ����
	TimeAccumulator += DeltaTime * CircleSpeed;

	// ���� ���� ���
	FVector NewLocation = StartLocation;
	NewLocation.X += CircleRadius * FMath::Cos(TimeAccumulator);
	NewLocation.Y += CircleRadius * FMath::Sin(TimeAccumulator);

	// Projectile �̵�
	SetActorLocation(NewLocation);
}

