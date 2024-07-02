// Fill out your copyright notice in the Description page of Project Settings.


#include "YJ/KirbyAttackAnimNotifyState.h"

void UKirbyAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	// 공격애니를 실행하면 스워드의 콜리전 활성화 
	GEngine->AddOnScreenDebugMessage(0, 2, FColor::Cyan, TEXT("NotifyBegin"));
}

void UKirbyAttackAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{

}

void UKirbyAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	GEngine->AddOnScreenDebugMessage(0, 2, FColor::Cyan, TEXT("NotifyEnd"));
	// 공격애니를 끝내면 스워드의 콜리전 비활성화 
}
