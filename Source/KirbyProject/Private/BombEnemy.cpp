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
#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"

ABombEnemy::ABombEnemy()
{
    PrimaryActorTick.bCanEverTick = true;

    BombRange = 1000.0f; // �÷��̾ �ٰ��;� �ϴ� �Ÿ�
    BombInterval = 1.70f; // ��ź spawn�Ǵ� ����
    Health = 3.0f;  // �ʱ� ü�� ����

    BombThrowDelay = 0.1f; // ��ź ��������� ��� �ð�
    HoldBomb = nullptr; // ����ִ� ��ź

    CurrentState = EEnemyState::Idle;  // �ʱ� ���¸� Idle�� ����
}

void ABombEnemy::BeginPlay()
{
    Super::BeginPlay();

    // PlayerPawn ������ �ʱ�ȭ�մϴ�
    PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    //GetWorldTimerManager().SetTimer(BombTimerHandle, this, &ABombEnemy::CheckBombCondition, BombInterval, true);

    OriginalMaterial = GetMesh()->GetMaterial(0);

    InitAnimation();
}

void ABombEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (PlayerPawn) {
        RotateToPlayer(DeltaTime);  // �� �����Ӹ��� �÷��̾ ���� ȸ��
    }

    CheckBombCondition();
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
            // attack���� �� BombInterval �ð� �ڿ� StartThrowingBomb���� ��ź spawn
            GetWorldTimerManager().SetTimer(BombTimerHandle, this, &ABombEnemy::StartThrowingBomb, BombInterval, true);
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

// Idle ----------------------------------------
void ABombEnemy::Idle()
{
    PlayAnimMontage(IdleAnimMontage);
}

// ��ź ������ -----------------------------------------------------------------
void ABombEnemy::CheckBombCondition()
{
    if (PlayerPawn)
    {
        float DistanceToPlayer = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());

        if (DistanceToPlayer <= BombRange)
        {
            SetState(EEnemyState::Attack);  // ���� ���·� ��ȯ
            
        }
        else
        {
            SetState(EEnemyState::Idle); // ���� �ȿ� ���� ��� Idle�� �ٲ�
        }
    }
}

void ABombEnemy::StartThrowingBomb()
{
    if(BombProjectileClass && !HoldBomb)
	{   // �� �� ��ź spawn �ص�
        FVector BombLocation = GetMesh()->GetSocketLocation(FName("BombEnemySocket"));
        FRotator BombRotation = GetMesh()->GetSocketRotation(FName("BombEnemySocket"));

        HoldBomb = GetWorld()->SpawnActor<ABombProjectile>(BombProjectileClass, BombLocation, BombRotation);

		if (HoldBomb) {
            // ��ź�� ������ �ð� �α� ���
            FDateTime Now = FDateTime::Now();
            FString NowStr = Now.ToString();
            UE_LOG(LogTemp, Log, TEXT("Bomb Spawned at: %s"), *NowStr);

			// ��� �ִٰ� ���� �ð� �Ŀ� ThrowBomb �Լ��� ȣ���ϴ� Ÿ�̸� ����
			GetWorldTimerManager().SetTimer(TimerHandle, this, &ABombEnemy::ThrowBomb, BombThrowDelay, false);
		}
	}
}

// �� �� ���� (���� �ð� �ڿ�)
void ABombEnemy::ThrowBomb()
{
    if (HoldBomb)
    {
        // ��ź ������ �ð� �α� ���
        FDateTime Now = FDateTime::Now();
        FString NowStr = Now.ToString();
        UE_LOG(LogTemp, Log, TEXT("Bomb Thrown at: %s"), *NowStr);

        // ��ź�� ���� �ӵ� �߰�
        FVector ForwardVelocity = GetActorForwardVector() * 800.0f;
        UPrimitiveComponent* BombComponent = Cast<UPrimitiveComponent>(HoldBomb->GetRootComponent());
        if (BombComponent)
        {
            BombComponent->SetSimulatePhysics(true);
            BombComponent->AddImpulse(ForwardVelocity, NAME_None, true);
        }
        
        // ��ź ���� �Ŀ� HoldBomb �ٽ� �ʱ�ȭ
        HoldBomb = nullptr;
    }
}


// Attack --------------------------------------------------------------------
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
    // ������ ���� �� �����̱� ����
    if(DamageMaterial)
    {
        GetMesh()->UPrimitiveComponent::SetMaterial(0, DamageMaterial);
    }

    KnockBack(this); // �˹�

    Health -= Damage;
    if (Health <= 0)
    {
        Die();
    }

    // ���� �ð� �Ŀ� ������ ���� ������ �ǵ����� Ÿ�̸� ����
    GetWorldTimerManager().SetTimer(TimerHandle, this, &ABombEnemy::ResetMaterial, 0.2f, false);
}

void ABombEnemy::Die()
{
    SetState(EEnemyState::Dead);

    // ���� �ùķ��̼� Ȱ��ȭ �� �� ���� -> ���ư��� ȿ��
    GetMesh()->SetSimulatePhysics(true);
    FVector LaunchDirection = FVector(FMath::RandRange(-1.0f, 1.0f), FMath::RandRange(-1.0f, 1.0f), 1.0f);
    LaunchDirection.Normalize();
    FVector LaunchForce = LaunchDirection * 1000.0f; // ���� ũ�� ����
    GetMesh()->AddImpulse(LaunchForce);

    // ���� �ð� �� �ı�
    SetLifeSpan(2.0f); // 2�� �Ŀ� �ı�

    if (CoinClass)
    {
        FVector SpawnLocation = GetActorLocation();
        FRotator SpawnRotation = GetActorRotation();
        GetWorld()->SpawnActor<AActor>(CoinClass, SpawnLocation, SpawnRotation);
    }
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
        Idle();
        break;
    case EEnemyState::Attack:
        // Attack ���¿��� ������ ����
        //RotateToPlayer(DeltaTime);
        PlayAnimMontage(AttackAnimMontage);
        break;
    case EEnemyState::Dead:
        // Dead ���¿��� ������ ����
        Die();
        break;
    default:
        break;
    }
}

//void ABombEnemy::StartBlinkEffect()
//{
//    if (UMaterialInstanceDynamic* MatInstance = GetMesh()->CreateAndSetMaterialInstanceDynamic(0))
//    {
//        MatInstance->SetScalarParameterValue(FName("BlinkAmount"), 1.0f); // �����̱� ����
//    }
//    GetWorldTimerManager().SetTimer(BlinkTimerHandle, this, &ABombEnemy::StopBlinkEffect, 2.0f, false); // 2�� �Ŀ� ������ ����
//}
//
//void ABombEnemy::StopBlinkEffect()
//{
//    if (UMaterialInstanceDynamic* MatInstance = GetMesh()->CreateAndSetMaterialInstanceDynamic(0))
//    {
//        MatInstance->SetScalarParameterValue(FName("BlinkAmount"), 0.0f); // ������ ����
//    }
//}

void ABombEnemy::ResetMaterial()
{
    if(OriginalMaterial)
    {
        GetMesh()->UPrimitiveComponent::SetMaterial(0, OriginalMaterial);
    }
}

void ABombEnemy::KnockBack(class AActor* actor)
{
    FVector LaunchVelocity = UKismetMathLibrary::GetDirectionUnitVector(actor->GetActorLocation(), GetActorLocation()) * (-500);
    
    
    if (UKismetSystemLibrary::IsValid(actor))
    {
        ACharacter::LaunchCharacter(LaunchVelocity, true, false);
    }

}
