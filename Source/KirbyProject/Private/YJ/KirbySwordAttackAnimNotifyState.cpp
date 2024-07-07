// Fill out your copyright notice in the Description page of Project Settings.


#include "YJ/KirbySwordAttackAnimNotifyState.h"
#include "KirbyProjectCharacter.h"
#include "Components/BoxComponent.h"

void UKirbySwordAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	AKirbyProjectCharacter* KirbyCha = Cast<AKirbyProjectCharacter>(MeshComp->GetOwner());

	//UBoxComponent* MeshComponent = KirbyCha->FindComponentByClass<UBoxComponent>();

	TArray<AActor*> AttachedActors;
	KirbyCha->GetAttachedActors(AttachedActors);

	for (AActor* AttachedActor : AttachedActors)
	{
		if (AttachedActor->ActorHasTag(FName("Sword")))
		{
			UBoxComponent* BoxComponent = AttachedActor->FindComponentByClass<UBoxComponent>();

			BoxComponent->SetBoxExtent(FVector(900, 900, 900));

			//�ݸ��� ä�� ���� ����

			BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
		}
	}


}

void UKirbySwordAttackAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
}

void UKirbySwordAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AKirbyProjectCharacter* KirbyCha = Cast<AKirbyProjectCharacter>(MeshComp->GetOwner());

	//�ٽ� ���󺹱�

	TArray<AActor*> AttachedActors;
	KirbyCha->GetAttachedActors(AttachedActors);

	for (AActor* AttachedActor : AttachedActors)
	{
		if (AttachedActor->ActorHasTag(FName("Sword")))
		{
			UBoxComponent* BoxComponent = AttachedActor->FindComponentByClass<UBoxComponent>();

			BoxComponent->SetBoxExtent(FVector(300, 300, 900));

			//�ݸ��� ä�� ���� ����

			BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Ignore);
		}
	}
}
