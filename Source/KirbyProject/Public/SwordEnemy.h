#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SwordEnemy.generated.h"


UCLASS()
class KIRBYPROJECT_API ASwordEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	ASwordEnemy();
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// 적 AI 관리 컴포넌트 클래스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSMComponent)
	class UEnemyFSM* fsm;
};
