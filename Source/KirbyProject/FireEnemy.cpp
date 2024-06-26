#include "FireEnemy.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h" // 이 파일은 FireProjectileClass가 사용하는 클래스의 헤더 파일이어야 합니다.


AFireEnemy::AFireEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	FireMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FireMesh"));
	FireMesh->SetupAttachment(Root);

	FireRange = 1000.0f; // 플레이어가 다가와야 하는 거리
	FireInterval = 1.0f; // 불을 쏘는 간격
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