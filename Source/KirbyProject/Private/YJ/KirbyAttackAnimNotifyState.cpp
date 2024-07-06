// Fill out your copyright notice in the Description page of Project Settings.


#include "YJ/KirbyAttackAnimNotifyState.h"
#include "KirbyProjectCharacter.h"

void UKirbyAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	// ���ݾִϸ� �����ϸ� �������� �ݸ��� Ȱ��ȭ 
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
	// ���ݾִϸ� ������ �������� �ݸ��� ��Ȱ��ȭ 

	AKirbyProjectCharacter* KirbyCha = Cast<AKirbyProjectCharacter>(MeshComp->GetOwner());

	if (KirbyCha)
	{
		if (KirbyCha->bSwordChainAttack_2)
		{
			//�ι�° �ִϸ��̼��� �����Ű���� 
			KirbyCha->OnChainAniDele.Broadcast();
		}
		KirbyCha->bSwordChainAttack_1 = true;
		KirbyCha->bSwordChainAttack_2 = false;
	}

	
}
