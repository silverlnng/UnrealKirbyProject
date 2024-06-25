#include "Fireball.h"

// Sets default values
AFireball::AFireball()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    if (!RootComponent)
    {
        RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
    }

    if (!CollisionComponent)
    {
        // 스피어를 단순 콜리전 표현으로 사용합니다.
        CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
        // 스피어의 콜리전 반경을 설정합니다.
        CollisionComponent->InitSphereRadius(15.0f);
        // 루트 컴포넌트가 콜리전 컴포넌트가 되도록 설정합니다.
        RootComponent = CollisionComponent;
    }

    if (!ProjectileMovementComponent)
    {
        // 이 컴포넌트를 사용하여 이 발사체의 이동을 주도합니다.
        ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
        ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
        ProjectileMovementComponent->InitialSpeed = 3000.0f;
        ProjectileMovementComponent->MaxSpeed = 3000.0f;
        ProjectileMovementComponent->bRotationFollowsVelocity = true;
        ProjectileMovementComponent->bShouldBounce = true;
        ProjectileMovementComponent->Bounciness = 0.3f;
        ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
    }

	/*FireballMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Fireball Mesh"));
	RootComponent = FireballMesh;
	
	FireballComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Fireball Component"));*/
	/*FireballComponent->InitialSpeed = 800;
	FireballComponent->MaxSpeed = 8500;*/
}

// Called when the game starts or when spawned
void AFireball::BeginPlay()
{
	Super::BeginPlay();

    check(GEngine != nullptr);

    // 디버그 메시지를 5초간 표시합니다. 
    // -1 '키' 값 실행인자가 메시지가 업데이트되거나 새로고침되지 않도록 방지합니다.
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using FPSCharacter."));
}

// Called every frame
void AFireball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 함수 기능을 입력에 바인딩하기 위해 호출
void AFireball::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    //// 'movement' 바인딩을 구성합니다.
    //PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
    //PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

    //// 'look' 바인딩을 구성합니다.
    //PlayerInputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
    //PlayerInputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);

    //// 'action' 바인딩을 구성합니다.
    //PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::StartJump);
    //PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::StopJump);
    //PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::Fire);
}

// 발사 방향으로의 발사체 속도를 초기화하는 함수입니다.
void AFireball::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AFireball::Fire()
{
    // 발사체 발사를 시도합니다.
    if (ProjectileClass)
    {
        // 카메라 트랜스폼을 구합니다.
        FVector CameraLocation;
        FRotator CameraRotation;
        GetActorEyesViewPoint(CameraLocation, CameraRotation);

        // MuzzleOffset이 카메라 살짝 앞에서 발사체를 스폰하도록 설정합니다.
        MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

        // MuzzleOffset을 카메라 스페이스에서 월드 스페이스로 변환합니다.
        FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

        // 조준이 살짝 위를 향하도록 왜곡합니다.
        FRotator MuzzleRotation = CameraRotation;
        MuzzleRotation.Pitch += 10.0f;

        UWorld* World = GetWorld();
        if (World)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = GetInstigator();

            // 총구에 발사체를 스폰합니다.
            AFPSProjectile* Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
            if (Projectile)
            {
                // 발사체의 초기 탄도를 설정합니다.
                FVector LaunchDirection = MuzzleRotation.Vector();
                Projectile->FireInDirection(LaunchDirection);
            }
        }
    }
}