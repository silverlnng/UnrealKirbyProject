#include "SwordEnemy.h"
#include "SwordEnemyFSM.h"
#include "UObject/ConstructorHelpers.h"


ASwordEnemy::ASwordEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// skeletal mesh ������ �ε�
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Game/Resource/Enemy/Sword/Animation/SwordEnemy.SwordEnemy"));
	
	// ������ �ε忡 �����ϸ�
	if (tempMesh.Succeeded()) {
		//������ �Ҵ�
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		// �޽� ��ġ �� ȸ�� ����
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, 0, 0));
		// �޽� ũ�� ����
		GetMesh()->SetRelativeScale3D(FVector(0.84f));
	}

	// EnemyFSM ������Ʈ �߰�
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