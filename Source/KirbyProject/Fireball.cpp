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
        // ���Ǿ �ܼ� �ݸ��� ǥ������ ����մϴ�.
        CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
        // ���Ǿ��� �ݸ��� �ݰ��� �����մϴ�.
        CollisionComponent->InitSphereRadius(15.0f);
        // ��Ʈ ������Ʈ�� �ݸ��� ������Ʈ�� �ǵ��� �����մϴ�.
        RootComponent = CollisionComponent;
    }

    if (!ProjectileMovementComponent)
    {
        // �� ������Ʈ�� ����Ͽ� �� �߻�ü�� �̵��� �ֵ��մϴ�.
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

    // ����� �޽����� 5�ʰ� ǥ���մϴ�. 
    // -1 'Ű' �� �������ڰ� �޽����� ������Ʈ�ǰų� ���ΰ�ħ���� �ʵ��� �����մϴ�.
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using FPSCharacter."));
}

// Called every frame
void AFireball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// �Լ� ����� �Է¿� ���ε��ϱ� ���� ȣ��
void AFireball::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    //// 'movement' ���ε��� �����մϴ�.
    //PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
    //PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

    //// 'look' ���ε��� �����մϴ�.
    //PlayerInputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
    //PlayerInputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);

    //// 'action' ���ε��� �����մϴ�.
    //PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::StartJump);
    //PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::StopJump);
    //PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::Fire);
}

// �߻� ���������� �߻�ü �ӵ��� �ʱ�ȭ�ϴ� �Լ��Դϴ�.
void AFireball::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AFireball::Fire()
{
    // �߻�ü �߻縦 �õ��մϴ�.
    if (ProjectileClass)
    {
        // ī�޶� Ʈ�������� ���մϴ�.
        FVector CameraLocation;
        FRotator CameraRotation;
        GetActorEyesViewPoint(CameraLocation, CameraRotation);

        // MuzzleOffset�� ī�޶� ��¦ �տ��� �߻�ü�� �����ϵ��� �����մϴ�.
        MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

        // MuzzleOffset�� ī�޶� �����̽����� ���� �����̽��� ��ȯ�մϴ�.
        FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

        // ������ ��¦ ���� ���ϵ��� �ְ��մϴ�.
        FRotator MuzzleRotation = CameraRotation;
        MuzzleRotation.Pitch += 10.0f;

        UWorld* World = GetWorld();
        if (World)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = GetInstigator();

            // �ѱ��� �߻�ü�� �����մϴ�.
            AFPSProjectile* Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
            if (Projectile)
            {
                // �߻�ü�� �ʱ� ź���� �����մϴ�.
                FVector LaunchDirection = MuzzleRotation.Vector();
                Projectile->FireInDirection(LaunchDirection);
            }
        }
    }
}