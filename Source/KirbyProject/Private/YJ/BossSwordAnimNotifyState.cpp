// Fill out your copyright notice in the Description page of Project Settings.


#include "YJ/BossSwordAnimNotifyState.h"

#include "Components/BoxComponent.h"
#include "YJ/BossEnemyCharacter.h"

void UBossSwordAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                            float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	ABossEnemyCharacter* bossCha = Cast<ABossEnemyCharacter>(MeshComp->GetOwner());
	if(bossCha)
	{
		TArray<AActor*> AttachedActors;
		bossCha->GetAttachedActors(AttachedActors);
		for (AActor* AttachedActor : AttachedActors)
		{
			if (AttachedActor->ActorHasTag(FName("BossSword")))
			{
				UBoxComponent* BoxComponent = AttachedActor->FindComponentByClass<UBoxComponent>();
				//콜리전 채널 설정 변경

				BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
			}
		}
	}
	
	
}

void UBossSwordAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	ABossEnemyCharacter* bossCha = Cast<ABossEnemyCharacter>(MeshComp->GetOwner());
	if(bossCha)
	{
		TArray<AActor*> AttachedActors;
		bossCha->GetAttachedActors(AttachedActors);
		for (AActor* AttachedActor : AttachedActors)
		{
			if (AttachedActor->ActorHasTag(FName("BossSword")))
			{
				UBoxComponent* BoxComponent = AttachedActor->FindComponentByClass<UBoxComponent>();
				//콜리전 채널 설정 변경

				BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
			}
		}
	}
}
