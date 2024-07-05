// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "KirbyProjectCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UENUM(BlueprintType) // BlueprintType을 사용하여 블루프린트에서 사용 가능하게 합니다.
enum class EState : uint8
{
    E_None UMETA(DisplayName = "E_None"), // 블루프린트에 표시될 이름을 지정합니다.
    E_Fire UMETA(DisplayName = "E_Fire"),
    E_Sword UMETA(DisplayName = "E_Sword"),
	E_Bomb UMETA(DisplayName = "E_Bomb")
};

UCLASS(config=Game)
class AKirbyProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

public:
	AKirbyProjectCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SwordAttack")
	EState state = EState::E_None;

	UFUNCTION(BlueprintCallable)
	void SwordAttack_started();

	UFUNCTION(BlueprintCallable)
	void SwordAttack_triggered();

	UFUNCTION(BlueprintCallable)
	void SwordAttack_completed();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SwordAttack")
	bool bSwordAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SwordAttack")
	bool bSwordAttacking_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SwordAttack")
	bool bSwordAttacking_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SwordAttack")
	bool bSwordAttacking_3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SwordAttack")
	bool bSwordSpecialAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SwordAttack")
	float SwordAttackTime =3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SwordAttack")
	float AttackPressTime =0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SwordAttack")
	class UAnimMontage* SwordAttackAnimMontage_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SwordAttack")
	class UAnimMontage* SwordAttackAnimMontage_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SwordAttack")
	class UAnimMontage* SwordAttackAnimMontage_3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SwordAttack")
	UInputAction* SwordAttackAction;
};

