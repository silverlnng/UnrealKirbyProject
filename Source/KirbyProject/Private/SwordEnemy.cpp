#include "SwordEnemy.h"
#include "SwordEnemyFSM.h"
#include "UObject/ConstructorHelpers.h"


ASwordEnemy::ASwordEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// skeletal mesh 데이터 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Game/Resource/Enemy/Sword/Animation/SwordEnemy.SwordEnemy"));
	
	// 데이터 로드에 성공하면
	if (tempMesh.Succeeded()) {
		//데이터 할당
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		// 메시 위치 및 회전 설정
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, 0, 0));
		// 메시 크기 수정
		GetMesh()->SetRelativeScale3D(FVector(0.84f));
	}

	// EnemyFSM 컴포넌트 추가
	fsm = CreateDefaultSubobject<USwordEnemyFSM>(TEXT("FSM"));
}

void ASwordEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void ASwordEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASwordEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}