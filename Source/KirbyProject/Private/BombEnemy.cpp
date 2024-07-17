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

    BombRange = 1000.0f; // 플레이어가 다가와야 하는 거리
    BombInterval = 1.70f; // 폭탄 spawn되는 간격
    Health = 3.0f;  // 초기 체력 설정

    BombThrowDelay = 0.1f; // 폭탄 던지기까지 대기 시간
    HoldBomb = nullptr; // 들고있는 폭탄

    CurrentState = EEnemyState::Idle;  // 초기 상태를 Idle로 설정
}

void ABombEnemy::BeginPlay()
{
    Super::BeginPlay();

    // PlayerPawn 변수를 초기화합니다
    PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    //GetWorldTimerManager().SetTimer(BombTimerHandle, this, &ABombEnemy::CheckBombCondition, BombInterval, true);

    OriginalMaterial = GetMesh()->GetMaterial(0);

    InitAnimation();
}

void ABombEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (PlayerPawn) {
        RotateToPlayer(DeltaTime);  // 매 프레임마다 플레이어를 향해 회전
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
            // attack시작 후 BombInterval 시간 뒤에 StartThrowingBomb으로 폭탄 spawn
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

// 폭탄 던지기 -----------------------------------------------------------------
void ABombEnemy::CheckBombCondition()
{
    if (PlayerPawn)
    {
        float DistanceToPlayer = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());

        if (DistanceToPlayer <= BombRange)
        {
            SetState(EEnemyState::Attack);  // 공격 상태로 전환
            
        }
        else
        {
            SetState(EEnemyState::Idle); // 범위 안에 없을 경우 Idle로 바뀜
        }
    }
}

void ABombEnemy::StartThrowingBomb()
{
    if(BombProjectileClass && !HoldBomb)
	{   // 이 때 폭탄 spawn 해둠
        FVector BombLocation = GetMesh()->GetSocketLocation(FName("BombEnemySocket"));
        FRotator BombRotation = GetMesh()->GetSocketRotation(FName("BombEnemySocket"));

        HoldBomb = GetWorld()->SpawnActor<ABombProjectile>(BombProjectileClass, BombLocation, BombRotation);

		if (HoldBomb) {
            // 폭탄을 생성한 시간 로그 출력
            FDateTime Now = FDateTime::Now();
            FString NowStr = Now.ToString();
            UE_LOG(LogTemp, Log, TEXT("Bomb Spawned at: %s"), *NowStr);

			// 들고 있다가 일정 시간 후에 ThrowBomb 함수를 호출하는 타이머 설정
			GetWorldTimerManager().SetTimer(TimerHandle, this, &ABombEnemy::ThrowBomb, BombThrowDelay, false);
		}
	}
}

// 이 때 던짐 (일정 시간 뒤에)
void ABombEnemy::ThrowBomb()
{
    if (HoldBomb)
    {
        // 폭탄 던지기 시간 로그 출력
        FDateTime Now = FDateTime::Now();
        FString NowStr = Now.ToString();
        UE_LOG(LogTemp, Log, TEXT("Bomb Thrown at: %s"), *NowStr);

        // 폭탄에 전방 속도 추가
        FVector ForwardVelocity = GetActorForwardVector() * 800.0f;
        UPrimitiveComponent* BombComponent = Cast<UPrimitiveComponent>(HoldBomb->GetRootComponent());
        if (BombComponent)
        {
            BombComponent->SetSimulatePhysics(true);
            BombComponent->AddImpulse(ForwardVelocity, NAME_None, true);
        }
        
        // 폭탄 던진 후에 HoldBomb 다시 초기화
        HoldBomb = nullptr;
    }
}


// Attack --------------------------------------------------------------------
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
    // 데미지 입을 때 깜박이기 시작
    if(DamageMaterial)
    {
        GetMesh()->UPrimitiveComponent::SetMaterial(0, DamageMaterial);
    }

    KnockBack(this); // 넉백

    Health -= Damage;
    if (Health <= 0)
    {
        Die();
    }

    // 일정 시간 후에 재질을 원래 재질로 되돌리는 타이머 설정
    GetWorldTimerManager().SetTimer(TimerHandle, this, &ABombEnemy::ResetMaterial, 0.2f, false);
}

void ABombEnemy::Die()
{
    SetState(EEnemyState::Dead);

    // 물리 시뮬레이션 활성화 및 힘 적용 -> 날아가는 효과
    GetMesh()->SetSimulatePhysics(true);
    FVector LaunchDirection = FVector(FMath::RandRange(-1.0f, 1.0f), FMath::RandRange(-1.0f, 1.0f), 1.0f);
    LaunchDirection.Normalize();
    FVector LaunchForce = LaunchDirection * 1000.0f; // 힘의 크기 조절
    GetMesh()->AddImpulse(LaunchForce);

    // 일정 시간 후 파괴
    SetLifeSpan(2.0f); // 2초 후에 파괴

    if (CoinClass)
    {
        FVector SpawnLocation = GetActorLocation();
        FRotator SpawnRotation = GetActorRotation();
        GetWorld()->SpawnActor<AActor>(CoinClass, SpawnLocation, SpawnRotation);
    }
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
        Idle();
        break;
    case EEnemyState::Attack:
        // Attack 상태에서 실행할 로직
        //RotateToPlayer(DeltaTime);
        PlayAnimMontage(AttackAnimMontage);
        break;
    case EEnemyState::Dead:
        // Dead 상태에서 실행할 로직
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
//        MatInstance->SetScalarParameterValue(FName("BlinkAmount"), 1.0f); // 깜박이기 시작
//    }
//    GetWorldTimerManager().SetTimer(BlinkTimerHandle, this, &ABombEnemy::StopBlinkEffect, 2.0f, false); // 2초 후에 깜박임 종료
//}
//
//void ABombEnemy::StopBlinkEffect()
//{
//    if (UMaterialInstanceDynamic* MatInstance = GetMesh()->CreateAndSetMaterialInstanceDynamic(0))
//    {
//        MatInstance->SetScalarParameterValue(FName("BlinkAmount"), 0.0f); // 깜박임 종료
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
