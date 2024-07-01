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

	if (PlayerPawn) {
		RotateToPlayer(DeltaTime);  // 매 프레임마다 플레이어를 향해 회전
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
		Direction.Z = 0;  // 수평 회전만 하도록 Z축을 0으로 설정

		if (!Direction.IsNearlyZero())
		{
			FRotator TargetRotation = Direction.Rotation();
			FRotator CurrentRotation = GetActorRotation();

			TargetRotation.Yaw += -90.0f;

			FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 5.0f);  // 회전 속도를 조정할 수 있습니다.
			SetActorRotation(NewRotation);
		}
	}
}