#include "SwordEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ASwordEnemy::ASwordEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	ChaseRange = 1000.0f; // �÷��̾ �Ѿư��� �����ϴ� �Ÿ�
	AttackRange = 200.0f; // ������ �����ϴ� �Ÿ�
	AttackInterval = 1.5f; // ���� ����
}

void ASwordEnemy::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	// ���� �������� �÷��̾���� �Ÿ� Ȯ��
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ASwordEnemy::CheckChaseConditoin, 0.5f, true);
}

void ASwordEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsPlayerInRange(ChaseRange) && !bIsPlayerInRange(AttackRange)) {
		// �÷��̾� �Ѿư�
		FVector Direction = PlayerPawn->GetActorLocation() - GetActorLocation();
		Direction.Z = 0.0f;
		//AddMovementInput(Direction.GetSafeNormal());
	}
}

bool ASwordEnemy::bIsPlayerInRange(float Range) {
	if (PlayerPawn) {
		float DistanceToPlayer = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());
		return DistanceToPlayer <= Range;
	}
	return false;
}

void ASwordEnemy::CheckChaseConditoin() {
	if (bIsPlayerInRange(ChaseRange)) {
		GetWorldTimerManager().ClearTimer(AttackTimerHandle);
		GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ASwordEnemy::CheckAttackCondition, AttackInterval, true);
	}
	else {
		GetWorldTimerManager().ClearTimer(AttackTimerHandle);
	}
}

void ASwordEnemy::CheckAttackCondition() {
	if (bIsPlayerInRange(AttackRange)) {
		Attack();
	}
}

void ASwordEnemy::Attack() {
	if (AttackMontage && bIsPlayerInRange(AttackRange)) {
		//PlayAnimMontage(AttackMontage);
	}
}