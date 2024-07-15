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

    BombRange = 1000.0f; // �÷��̾ �ٰ��;� �ϴ� �Ÿ�
    BombInterval = 0.5f; // ���� ��� ����
    Health = 3.0f;  // �ʱ� ü�� ����

    //// �ݸ��� ĸ�� �߰�
    //DetectionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DetectionCapsule"));
    //DetectionCapsule->InitCapsuleSize(55.0f, 96.0f);
    //DetectionCapsule->SetCollisionProfileName(TEXT("Trigger"));
    //DetectionCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    //DetectionCapsule->OnComponentBeginOverlap.AddDynamic(this, &ABombEnemy::OnOverlapBegin);

    CurrentState = EEnemyState::Idle;  // �ʱ� ���¸� Idle�� ����
}

void ABombEnemy::BeginPlay()
{
    Super::BeginPlay();

    // PlayerPawn ������ �ʱ�ȭ�մϴ�
    PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    GetWorldTimerManager().SetTimer(BombTimerHandle, this, &ABombEnemy::CheckBombCondition, BombInterval, true);

    InitAnimation();
}

void ABombEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (PlayerPawn) {
        RotateToPlayer(DeltaTime);  // �� �����Ӹ��� �÷��̾ ���� ȸ��
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
            SetState(EEnemyState::Idle); // ���� �ȿ� ���� ��� Idle�� �ٲ�
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
        Direction.Z = 0;  // ���� ȸ���� �ϵ��� Z���� 0���� ����

        if (!Direction.IsNearlyZero())
        {
            FRotator TargetRotation = Direction.Rotation();
            FRotator CurrentRotation = GetActorRotation();

            TargetRotation.Yaw += -180.0f;

            FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 5.0f);  // ȸ�� �ӵ��� ������ �� �ֽ��ϴ�.
            SetActorRotation(NewRotation);
        }
    }
}

void ABombEnemy::OnHit(float Damage)
{
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), StarVFX, GetActorLocation());
    StartBlinkEffect(); // ������ ���� �� �����̱� ����
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

    // ���� �ùķ��̼� Ȱ��ȭ �� �� ���� -> ���ư��� ȿ��
    GetMesh()->SetSimulatePhysics(true);
    FVector LaunchDirection = FVector(FMath::RandRange(-1.0f, 1.0f), FMath::RandRange(-1.0f, 1.0f), 1.0f);
    LaunchDirection.Normalize();
    FVector LaunchForce = LaunchDirection * 1000.0f; // ���� ũ�� ����
    GetMesh()->AddImpulse(LaunchForce);

    // ���� �ð� �� �ı�
    SetLifeSpan(1.0f); // 1�� �Ŀ� �ı�

    // ���� �� ��
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SmokeVFX, GetActorLocation());
    Destroy();  // ���� ����
}

// �� �κ��� ��� �� �ϴ� �� ������...............�³�
void ABombEnemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult)
{
    // �÷��̾�� �浹 �� �������� ��
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        AKirbyProjectCharacter* Player = Cast<AKirbyProjectCharacter>(OtherActor);
        if (Player)
        {
            // �÷��̾�� �������� �� (�ʿ��� ������ ó�� ���� �߰�)
            UGameplayStatics::ApplyDamage(Player, 10.0f, nullptr, this, nullptr);
        }
    }
}

void ABombEnemy::ApplySuckingForce()
{
	//if (GetMesh)
	//{
		FVector ForceDirection = (PlayerPawn->GetActorLocation() - GetMesh()->GetComponentLocation()).GetSafeNormal();
		float ForceMagnitude = 2000.0f; // ������ ���� ũ�� ����
		FVector Force = ForceDirection * ForceMagnitude;

        GetMesh()->AddForce(Force);
    //}
}

// �ִϸ��̼�---------------------------------------------------------------------
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
        // Idle ���¿��� ������ ����
        break;
    case EEnemyState::Attack:
        // Attack ���¿��� ������ ����
        RotateToPlayer(DeltaTime);
        break;
    case EEnemyState::Dead:
        // Dead ���¿��� ������ ����
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
        MatInstance->SetScalarParameterValue(FName("BlinkAmount"), 1.0f); // �����̱� ����
    }
    GetWorldTimerManager().SetTimer(BlinkTimerHandle, this, &ABombEnemy::StopBlinkEffect, 2.0f, false); // 2�� �Ŀ� ������ ����
}

void ABombEnemy::StopBlinkEffect()
{
    if (UMaterialInstanceDynamic* MatInstance = GetMesh()->CreateAndSetMaterialInstanceDynamic(0))
    {
        MatInstance->SetScalarParameterValue(FName("BlinkAmount"), 0.0f); // ������ ����
    }
}