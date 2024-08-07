// Fill out your copyright notice in the Description page of Project Settings.


#include "BombProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ABombProjectile::ABombProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = CollisionComponent;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 3000.0f;
	ProjectileMovement->MaxSpeed = 3000.0f;

}

// Called when the game starts or when spawned
void ABombProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABombProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

