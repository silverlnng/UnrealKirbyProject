#include "BombEnemy.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "BombProjectile.h"
#include "KirbyProjectCharacter.h"
#include "Components/CapsuleComponent.h"
#include "IdleAnimNotify.h"
#include "AttackAnimNotify.h"
#include "DeathAnimNotify.h"
#include "Materials/MaterialInstanceDynamic.h"

ABombEnemy::ABombEnemy()
{
    PrimaryActorTick.bCanEverTick = true;

    //BombMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BombMesh"));

    //BombMesh->SetSimulatePhysics(true);
    //BombMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    BombRange = 1000.0f; // 플레이어가 다가와야 하는 거리
    BombInterval = 0.5f; // 불을 쏘는 간격
    Health = 3.0f;  // 초기 체력 설정

    //// 콜리전 캡슐 추가
    //DetectionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DetectionCapsule"));
    //DetectionCapsule->InitCapsuleSize(55.0f, 96.0f);
    //DetectionCapsule->SetCollisionProfileName(TEXT("Trigger"));
    //DetectionCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    //DetectionCapsule->OnComponentBeginOverlap.AddDynamic(this, &ABombEnemy::OnOverlapBegin);

    CurrentState = EEnemyState::Idle;  // 초기 상태를 Idle로 설정
}

void ABombEnemy::BeginPlay()
{
    Super::BeginPlay();

    // PlayerPawn 변수를 초기화합니다
    PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    GetWorldTimerManager().SetTimer(BombTimerHandle, this, &ABombEnemy::CheckBombCondition, BombInterval, true);

    InitAnimation();
}

void ABombEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (PlayerPawn) {
        RotateToPlayer(DeltaTime);  // 매 프레임마다 플레이어를 향해 회전
    }

    UpdateAnimation(DeltaTime);
}

void ABombEnemy::SetState(EEnemyState NewState)
{
    if (CurrentState != NewState)
    {
        CurrentState = NewState;
        switch (NewState)
        {
        case EEnemyState::Idle:
            Idle();
            break;
        case EEnemyState::Attack:
            PlayAnimMontage(AttackAnimMontage);
            break;
        case EEnemyState::Dead:
            PlayAnimMontage(DeathAnimMontage);
            Die();
            break;
        default:
            break;
        }
    }
}

void ABombEnemy::Idle()
{
    PlayAnimMontage(IdleAnimMontage);
}

void ABombEnemy::CheckBombCondition()
{
    if (PlayerPawn)
    {
        float DistanceToPlayer = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());

        if (DistanceToPlayer <= BombRange)
        {
            Bomb();
            SetState(EEnemyState::Attack);
        }
        else
        {
            SetState(EEnemyState::Idle); // 범위 안에 없을 경우 Idle로 바뀜
        }
    }
}

void ABombEnemy::Bomb()
{
    if (BombProjectileClass)
    {
        FVector BombLocation = GetMesh()->GetSocketLocation(FName("BombEnemySocket"));
        FRotator BombRotation = GetMesh()->GetSocketRotation(FName("BombEnemySocket"));

        GetWorld()->SpawnActor<ABombProjectile>(BombProjectileClass, BombLocation, BombRotation);
    }
}

// Attack
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

            TargetRotation.Yaw += -180.0f;

            FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 5.0f);  // 회전 속도를 조정할 수 있습니다.
            SetActorRotation(NewRotation);
        }
    }
}

void ABombEnemy::OnHit(float Damage)
{
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), StarVFX, GetActorLocation());
    StartBlinkEffect(); // 데미지 입을 때 깜박이기 시작
    Health -= Damage;
    if (Health <= 0)
    {
        Die();
    }
}

void ABombEnemy::Die()
{
    SetState(EEnemyState::Dead);

    if (CoinClass)
    {
        FVector SpawnLocation = GetActorLocation();
        FRotator SpawnRotation = GetActorRotation();
        GetWorld()->SpawnActor<AActor>(CoinClass, SpawnLocation, SpawnRotation);
    }

    // 물리 시뮬레이션 활성화 및 힘 적용 -> 날아가는 효과
    GetMesh()->SetSimulatePhysics(true);
    FVector LaunchDirection = FVector(FMath::RandRange(-1.0f, 1.0f), FMath::RandRange(-1.0f, 1.0f), 1.0f);
    LaunchDirection.Normalize();
    FVector LaunchForce = LaunchDirection * 1000.0f; // 힘의 크기 조절
    GetMesh()->AddImpulse(LaunchForce);

    // 일정 시간 후 파괴
    SetLifeSpan(1.0f); // 1초 후에 파괴

    // 죽을 때 펑
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SmokeVFX, GetActorLocation());
    Destroy();  // 적을 제거
}

// 이 부분은 사용 안 하는 것 같은데...............맞나
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

void ABombEnemy::ApplySuckingForce()
{
	//if (GetMesh)
	//{
		FVector ForceDirection = (PlayerPawn->GetActorLocation() - GetMesh()->GetComponentLocation()).GetSafeNormal();
		float ForceMagnitude = 2000.0f; // 적절한 힘의 크기 설정
		FVector Force = ForceDirection * ForceMagnitude;

        GetMesh()->AddForce(Force);
    //}
}

// 애니메이션---------------------------------------------------------------------
void ABombEnemy::InitAnimation()
{
    if (IdleAnimMontage)
    {
        const auto NotifyEvents = IdleAnimMontage->Notifies;
        for (FAnimNotifyEvent EventNotify : NotifyEvents)
        {
            if (const auto IdleNotify = Cast<UIdleAnimNotify>(EventNotify.Notify))
            {
                IdleNotify->OnNotified.AddUObject(this, &ABombEnemy::IdleAnimNotify);
            }
        }
    }
    if (AttackAnimMontage)
    {
        const auto NotifyEvents = AttackAnimMontage->Notifies;
        for (FAnimNotifyEvent EventNotify : NotifyEvents)
        {
            if (const auto AttackNotify = Cast<UAttackAnimNotify>(EventNotify.Notify))
            {
                AttackNotify->OnNotified.AddUObject(this, &ABombEnemy::AttackAnimNotify);
            }
        }
    }
    if (DeathAnimMontage)
    {
        const auto NotifyEvents = DeathAnimMontage->Notifies;
        for (FAnimNotifyEvent EventNotify : NotifyEvents)
        {
            if (const auto DeathNotify = Cast<UDeathAnimNotify>(EventNotify.Notify))
            {
                DeathNotify->OnNotified.AddUObject(this, &ABombEnemy::DeathAnimNotify);
            }
        }
    }
}

void ABombEnemy::IdleAnimNotify()
{
    UE_LOG(LogTemp, Warning, TEXT("Idle Anim was finished"));
}

void ABombEnemy::AttackAnimNotify()
{
    UE_LOG(LogTemp, Warning, TEXT("Attack Anim was finished"));
}

void ABombEnemy::DeathAnimNotify()
{
    UE_LOG(LogTemp, Warning, TEXT("Death Anim was finished"));
}


void ABombEnemy::PlayAnimMontage(UAnimMontage* MontageToPlay)
{
    if (GetMesh() && MontageToPlay)
    {
        GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay);
    }
}

void ABombEnemy::UpdateAnimation(float DeltaTime)
{
    switch (CurrentState)
    {
    case EEnemyState::Idle:
        // Idle 상태에서 실행할 로직
        break;
    case EEnemyState::Attack:
        // Attack 상태에서 실행할 로직
        RotateToPlayer(DeltaTime);
        break;
    case EEnemyState::Dead:
        // Dead 상태에서 실행할 로직
        Die();
        break;
    default:
        break;
    }
}

void ABombEnemy::StartBlinkEffect()
{
    if (UMaterialInstanceDynamic* MatInstance = GetMesh()->CreateAndSetMaterialInstanceDynamic(0))
    {
        MatInstance->SetScalarParameterValue(FName("BlinkAmount"), 1.0f); // 깜박이기 시작
    }
    GetWorldTimerManager().SetTimer(BlinkTimerHandle, this, &ABombEnemy::StopBlinkEffect, 2.0f, false); // 2초 후에 깜박임 종료
}

void ABombEnemy::StopBlinkEffect()
{
    if (UMaterialInstanceDynamic* MatInstance = GetMesh()->CreateAndSetMaterialInstanceDynamic(0))
    {
        MatInstance->SetScalarParameterValue(FName("BlinkAmount"), 0.0f); // 깜박임 종료
    }
}