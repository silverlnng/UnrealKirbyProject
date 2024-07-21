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

	FireRange = 1000.0f; // �÷��̾ �ٰ��;� �ϴ� �Ÿ�
	FireInterval = 1.5f; // ���� ��� ����
	Health = 3.0f;  // �ʱ� ü�� ����

	// Ÿ�Ӷ��� ������Ʈ �ʱ�ȭ -------------------------------------------------------------
	FireTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("FireTimeline"));

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Resource/Enemy/Fire/CannonScaleCurve.CannonScaleCurve"));
	if (Curve.Succeeded())
	{
		ScaleCurve = Curve.Object;
	}

	// ����� ������Ʈ �ʱ�ȭ -------------------------------------------------------------
	FireAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FireAudioComponent"));
	FireAudioComponent->SetupAttachment(Root);
	FireAudioComponent->bAutoActivate = false; // �ʱ⿡�� ��Ȱ��ȭ ����

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
	//	RotateToPlayer(DeltaTime);  // �� �����Ӹ��� �÷��̾ ���� ȸ��
	//}

	// FireTimeline ������Ʈ------------------------------------------------------------
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
			// �������� �����մϴ�.
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

		// Ÿ�Ӷ��� ���------------------------
		if (FireTimeline)
		{
			FireTimeline->PlayFromStart();
		}

		// 2�� 0.5�� �������� �߻��ϰ�, 2�� ���� �ݺ�
		ProjectileCount++;
		if (ProjectileCount >= 2)
		{
			StopSequence();
		}

		// �߻� �Ҹ�
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

	// ���� ����
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

	Destroy();  // ���� ����
}

//------------------------------------------------------
void AFireEnemy::UpdateScale(float ScaleValue)
{
	FireMesh->SetRelativeScale3D(FVector(ScaleValue));
}

