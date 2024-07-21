#include "FireEnemy.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h" 
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"


AFireEnemy::AFireEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Root->SetBoxExtent(FVector(75, 75, 75));

	FireMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FireMesh"));
	FireMesh->SetupAttachment(Root);

	FireRange = 2000.0f; // 플레이어가 다가와야 하는 거리
	FireInterval = 1.5f; // 불을 쏘는 간격
	Health = 3.0f;  // 초기 체력 설정

	// 타임라인 컴포넌트 초기화 -------------------------------------------------------------
	FireTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("FireTimeline"));

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Resource/Enemy/Fire/CannonScaleCurve.CannonScaleCurve"));
	if (Curve.Succeeded())
	{
		ScaleCurve = Curve.Object;
	}

	ProjectileCount = 0;
}

void AFireEnemy::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AFireEnemy::CheckFireCondition, FireInterval, true);

	// --------------------------------------------------------------------
	if (ScaleCurve)
	{
		FOnTimelineFloat TimelineCallback;
		TimelineCallback.BindUFunction(this, FName("UpdateScale"));
		FireTimeline->AddInterpFloat(ScaleCurve, TimelineCallback);
	}
}

void AFireEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (PlayerPawn) {
	//	RotateToPlayer(DeltaTime);  // 매 프레임마다 플레이어를 향해 회전
	//}

	// FireTimeline 업데이트------------------------------------------------------------
	if (FireTimeline)
	{
		FireTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);
	}
}

void AFireEnemy::CheckFireCondition()
{
	if (PlayerPawn)
	{
		float DistanceToPlayer = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());

		if (DistanceToPlayer <= FireRange)
		{
			// 시퀀스를 시작합니다.
			StartSequence();
		}
	}
}

void AFireEnemy::Fire()
{
	if (FireProjectileClass)
	{
		FVector FireLocation = FireMesh->GetSocketLocation(FName("FireSocket"));
		FRotator FireRotation = FireMesh->GetSocketRotation(FName("FireSocket"));

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionVFX, FireMesh->GetSocketLocation(FName("FireSocket")));

		GetWorld()->SpawnActor<AProjectile>(FireProjectileClass, FireLocation, FireRotation);

		// 타임라인 재생------------------------
		if (FireTimeline)
		{
			FireTimeline->PlayFromStart();
		}

		// 2번 0.5초 간격으로 발사하고, 2초 쉬고 반복
		ProjectileCount++;
		if (ProjectileCount >= 2)
		{
			StopSequence();
		}

		// 발사 소리
		UGameplayStatics::PlaySound2D( GetWorld() , ExplosionSound );
	}
}

void AFireEnemy::StartSequence()
{
	ProjectileCount = 0;
	GetWorldTimerManager().SetTimer(ProjectileTimerHandle, this, &AFireEnemy::Fire, 0.5f, true, 0.0f);
}

void AFireEnemy::StopSequence()
{
	GetWorldTimerManager().ClearTimer(ProjectileTimerHandle);
	GetWorldTimerManager().SetTimer(SequenceTimerHandle, this, &AFireEnemy::StartSequence, 2.0f, false);
}

//void AFireEnemy::RotateToPlayer(float DeltaTime)
//{
//	if (PlayerPawn)
//	{
//		FVector Direction = PlayerPawn->GetActorLocation() - GetActorLocation();
//		Direction.Z = 0;  // 수평 회전만 하도록 Z축을 0으로 설정
//
//		if (!Direction.IsNearlyZero())
//		{
//			FRotator TargetRotation = Direction.Rotation();
//			FRotator CurrentRotation = GetActorRotation();
//
//			TargetRotation.Yaw += -90.0f;
//
//			FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 2.0f);  // 회전 속도를 조정할 수 있습니다.
//			SetActorRotation(NewRotation);
//		}
//	}
//}

//void AFireEnemy::OnHit(float Damage)
//{
//	Health -= Damage;
//	if (Health <= 0)
//	{
//		Die();
//	}
//}

void AFireEnemy::Die()
{
	if (CoinClass)
	{
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();
		GetWorld()->SpawnActor<AActor>(CoinClass, SpawnLocation, SpawnRotation);
	}

	Destroy();  // 적을 제거
}

//------------------------------------------------------
void AFireEnemy::UpdateScale(float ScaleValue)
{
	FireMesh->SetRelativeScale3D(FVector(ScaleValue));
}