#include "FireEnemy.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h" 
#include "Components/TimelineComponent.h"
#include "Components/AudioComponent.h"
#include "Curves/CurveFloat.h"


AFireEnemy::AFireEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Root->SetBoxExtent(FVector(75, 75, 75));

	FireMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FireMesh"));
	FireMesh->SetupAttachment(Root);

	FireRange = 1000.0f; // 플레이어가 다가와야 하는 거리
	FireInterval = 1.5f; // 불을 쏘는 간격
	Health = 3.0f;  // 초기 체력 설정

	// 타임라인 컴포넌트 초기화 -------------------------------------------------------------
	FireTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("FireTimeline"));

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Resource/Enemy/Fire/CannonScaleCurve.CannonScaleCurve"));
	if (Curve.Succeeded())
	{
		ScaleCurve = Curve.Object;
	}

	// 오디오 컴포넌트 초기화 -------------------------------------------------------------
	FireAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FireAudioComponent"));
	FireAudioComponent->SetupAttachment(Root);
	FireAudioComponent->bAutoActivate = false; // 초기에는 비활성화 상태

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
		else
		{
			StopSequence();
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
		if (FireAudioComponent)
		{
			FireAudioComponent->SetSound(ExplosionSound);
			FireAudioComponent->Play();
		}
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

	// 사운드 중지
	if (FireAudioComponent && FireAudioComponent->IsPlaying())
	{
		FireAudioComponent->Stop();
	}
}


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

