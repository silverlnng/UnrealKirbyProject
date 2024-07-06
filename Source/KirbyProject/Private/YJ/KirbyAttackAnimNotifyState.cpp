// Fill out your copyright notice in the Description page of Project Settings.


#include "YJ/KirbyAttackAnimNotifyState.h"
#include "KirbyProjectCharacter.h"

void UKirbyAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	// 공격애니를 실행하면 스워드의 콜리전 활성화 
	//GEngine->AddOnScreenDebugMessage(0, 2, FColor::Cyan, TEXT("NotifyBegin"));

	AKirbyProjectCharacter* KirbyCha = Cast<AKirbyProjectCharacter>(MeshComp->GetOwner());
	if (KirbyCha)
	{
	}
	//KirbyCha->

}

void UKirbyAttackAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	
}

void UKirbyAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	//GEngine->AddOnScreenDebugMessage(0, 2, FColor::Cyan, TEXT("NotifyEnd"));
	// 공격애니를 끝내면 스워드의 콜리전 비활성화 

	AKirbyProjectCharacter* KirbyCha = Cast<AKirbyProjectCharacter>(MeshComp->GetOwner());

	if (KirbyCha)
	{
		if (KirbyCha->bSwordChainAttack_2)
		{
			//두번째 애니메이션을 실행시키도록 
			KirbyCha->OnChainAniDele.Broadcast();
		}
		KirbyCha->bSwordChainAttack_1 = true;
		KirbyCha->bSwordChainAttack_2 = false;
	}

	
}
