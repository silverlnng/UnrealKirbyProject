#include "FireEnemy.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h" // �� ������ FireProjectileClass�� ����ϴ� Ŭ������ ��� �����̾�� �մϴ�.


AFireEnemy::AFireEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	FireMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FireMesh"));
	FireMesh->SetupAttachment(Root);

	FireRange = 1000.0f; // �÷��̾ �ٰ��;� �ϴ� �Ÿ�
	FireInterval = 1.0f; // ���� ��� ����
}

void AFireEnemy::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AFireEnemy::CheckFireCondition, FireInterval, true);
}

void AFireEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerPawn) {
		RotateToPlayer(DeltaTime);  // �� �����Ӹ��� �÷��̾ ���� ȸ��
	}

}

void AFireEnemy::CheckFireCondition()
{
	if (PlayerPawn)
	{
		float DistanceToPlayer = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());

		if (DistanceToPlayer <= FireRange)
		{
			Fire();
		}
	}
}

void AFireEnemy::Fire()
{
	if (FireProjectileClass)
	{
		FVector FireLocation = FireMesh->GetSocketLocation(FName("FireEnemySocket"));
		FRotator FireRotation = FireMesh->GetSocketRotation(FName("FireEnemySocket"));

		GetWorld()->SpawnActor<AProjectile>(FireProjectileClass, FireLocation, FireRotation);
	}
}

void AFireEnemy::RotateToPlayer(float DeltaTime)
{
	if (PlayerPawn)
	{
		FVector Direction = PlayerPawn->GetActorLocation() - GetActorLocation();
		Direction.Z = 0;  // ���� ȸ���� �ϵ��� Z���� 0���� ����

		if (!Direction.IsNearlyZero())
		{
			FRotator TargetRotation = Direction.Rotation();
			FRotator CurrentRotation = GetActorRotation();

			TargetRotation.Yaw += -90.0f;

			FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 5.0f);  // ȸ�� �ӵ��� ������ �� �ֽ��ϴ�.
			SetActorRotation(NewRotation);
		}
	}
}