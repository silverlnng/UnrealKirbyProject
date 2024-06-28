#include "SwordEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ASwordEnemy::ASwordEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	ChaseRange = 1000.0f; // 플레이어를 쫓아가기 시작하는 거리
	AttackRange = 200.0f; // 공격을 시작하는 거리
	AttackInterval = 1.5f; // 공격 간격
}

void ASwordEnemy::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	// 일정 간격으로 플레이어와의 거리 확인
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ASwordEnemy::CheckChaseConditoin, 0.5f, true);
}

void ASwordEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsPlayerInRange(ChaseRange) && !bIsPlayerInRange(AttackRange)) {
		// 플레이어 쫓아감
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