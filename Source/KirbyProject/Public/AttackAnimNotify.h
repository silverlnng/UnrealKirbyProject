#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AttackAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnMotifiedSignature);

UCLASS()
class KIRBYPROJECT_API UAttackAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	FOnMotifiedSignature OnNotified;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
