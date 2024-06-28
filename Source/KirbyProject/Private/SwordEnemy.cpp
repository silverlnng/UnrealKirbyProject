#include "SwordEnemy.h"
#include "SwordEnemyFSM.h"

ASwordEnemy::ASwordEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// skeletal mesh ������ �ε�
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Resource/Enemy/Sword/Animation/SwordEnemy.SwordEnemy'"));
	
	// ������ �ε忡 �����ϸ�
	if (tempMesh.Succeeded()) {
		//������ �Ҵ�
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		// �޽� ��ġ �� ȸ�� ����
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
		// �޽� ũ�� ����
		GetMesh()->SetRelativeScale3D(FVector(0.84f));
	}

	// EnemyFSM ������Ʈ �߰�
	fsm = CreateDefaultSubobject<USwordEnemy>(TEXT("FSM"));
};