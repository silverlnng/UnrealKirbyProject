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