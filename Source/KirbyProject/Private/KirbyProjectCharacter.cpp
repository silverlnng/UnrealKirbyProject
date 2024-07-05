// Copyright Epic Games, Inc. All Rights Reserved.

#include "KirbyProjectCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AKirbyProjectCharacter

AKirbyProjectCharacter::AKirbyProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	PrimaryActorTick.bCanEverTick = true;
}

void AKirbyProjectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}



void AKirbyProjectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if (bSwordAttacking)
	{
		AttackPressTime += DeltaTime;
		//
	}
	

}

//////////////////////////////////////////////////////////////////////////
// Input

void AKirbyProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AKirbyProjectCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AKirbyProjectCharacter::Look);

		// 스워드 공격
		EnhancedInputComponent->BindAction(SwordAttackAction, ETriggerEvent::Started, this, &AKirbyProjectCharacter::SwordAttack_started);
		EnhancedInputComponent->BindAction(SwordAttackAction, ETriggerEvent::Triggered, this, &AKirbyProjectCharacter::SwordAttack_triggered);
		EnhancedInputComponent->BindAction(SwordAttackAction, ETriggerEvent::Completed, this, &AKirbyProjectCharacter::SwordAttack_completed);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AKirbyProjectCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AKirbyProjectCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AKirbyProjectCharacter::SwordAttack_started()
{
	GEngine->AddOnScreenDebugMessage(0, 2, FColor::Cyan, TEXT("SwordAttack_started"));
	// 타이머 작동
	bSwordAttacking = true;
	PlayAnimMontage(SwordAttackAnimMontage_1);

	// 첫번째 start 입력과 두번째 start 입력 간의 시간을 계산하기 
	//bSwordAttacking_1 = true;
}

void AKirbyProjectCharacter::SwordAttack_triggered()
{
	// 버튼을 일정시간 이상 동안 누르는 중이면 필살기를 위한 대기자세(애니메이션2)를 LOOP 로 실행 
	UE_LOG(LogTemp, Warning, TEXT("%f"), AttackPressTime);

	if (AttackPressTime >= SwordAttackTime*0.5f)
	{
		PlayAnimMontage(SwordAttackAnimMontage_2);
	}

	// 아니라면 단순 공격

}

void AKirbyProjectCharacter::SwordAttack_completed()
{
	bSwordAttacking = false;		//타이머 종료


	//AttackPressTime  3초 이상이면 필살기 공격까지 
		// 아니라면 단순공격임 아무런 일도 없어야
	if (AttackPressTime >= SwordAttackTime)
	{
		PlayAnimMontage(SwordAttackAnimMontage_3);
	}
	else
	{
		PlayAnimMontage(SwordAttackAnimMontage_1);
	}
	UE_LOG(LogTemp, Warning, TEXT("%f"), AttackPressTime);

	AttackPressTime = 0.f;		//타이머 리셋
}
