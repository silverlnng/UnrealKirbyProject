#include "FireEnemy.h"

AFireEnemy::AFireEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	FireMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FireMesh"));
	FireMesh->SetupAttachment(Root);
}

void AFireEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFireEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

