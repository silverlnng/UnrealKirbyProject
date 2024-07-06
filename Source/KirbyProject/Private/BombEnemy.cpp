#include "BombEnemy.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "BombProjectile.h"
#include "KirbyProjectCharacter.h"
#include "Components/CapsuleComponent.h"

ABombEnemy::ABombEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	BombMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BombMesh"));
	BombMesh->SetupAttachment(Root);
	BombMesh->SetSimulatePhysics(true);
	BombMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	BombRange = 1000.0f; // 플레이어가 다가와야 하는 거리
	BombInterval = 1.0f; // 불을 쏘는 간격
	Health = 3.0f;  // 초기 체력 설정

	// 콜리전 캡슐 추가
	DetectionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DetectionCapsule"));
	DetectionCapsule->SetupAttachment(Root);
	DetectionCapsule->InitCapsuleSize(55.0f, 96.0f);
	DetectionCapsule->SetCollisionProfileName(TEXT("Trigger"));
	DetectionCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	DetectionCapsule->OnComponentBeginOverlap.AddDynamic(this, &ABombEnemy::OnOverlapBegin);
}

void ABombEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	GetWorldTimerManager().SetTimer(BombTimerHandle, this, &ABombEnemy::CheckBombCondition, BombInterval, true);
}

void ABombEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerPawn) {
		RotateToPlayer(DeltaTime);  // 매 프레임마다 플레이어를 향해 회전
	}
}

void ABombEnemy::CheckBombCondition()
{
	if (PlayerPawn)
	{
		float DistanceToPlayer = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());

		if (DistanceToPlayer <= BombRange)
		{
			Bomb();
		}
	}
}

void ABombEnemy::Bomb()
{
	if (BombProjectileClass)
	{
		FVector BombLocation = BombMesh->GetSocketLocation(FName("BombEnemySocket"));
		FRotator BombRotation = BombMesh->GetSocketRotation(FName("BombEnemySocket"));

		GetWorld()->SpawnActor<ABombProjectile>(BombProjectileClass, BombLocation, BombRotation);
	}
}

void ABombEnemy::RotateToPlayer(float DeltaTime)
{
	if (PlayerPawn)
	{
		FVector Direction = PlayerPawn->GetActorLocation() - GetActorLocation();
		Direction.Z = 0;  // 수평 회전만 하도록 Z축을 0으로 설정

		if (!Direction.IsNearlyZero())
		{
			FRotator TargetRotation = Direction.Rotation();
			FRotator CurrentRotation = GetActorRotation();

			TargetRotation.Yaw += -120.0f;

			FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 5.0f);  // 회전 속도를 조정할 수 있습니다.
			SetActorRotation(NewRotation);
		}
	}
}

void ABombEnemy::OnHit(float Damage)
{
	Health -= Damage;
	if (Health <= 0)
	{
		Die();
	}
}

void ABombEnemy::Die()
{
	if (CoinClass)
	{
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();
		GetWorld()->SpawnActor<AActor>(CoinClass, SpawnLocation, SpawnRotation);
	}

	Destroy();  // 적을 제거
}

void ABombEnemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	// 플레이어와 충돌 시 데미지를 줌
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		AKirbyProjectCharacter* Player = Cast<AKirbyProjectCharacter>(OtherActor);
		if (Player)
		{
			// 플레이어에게 데미지를 줌 (필요한 데미지 처리 로직 추가)
			UGameplayStatics::ApplyDamage(Player, 10.0f, nullptr, this, nullptr);
		}
	}
}