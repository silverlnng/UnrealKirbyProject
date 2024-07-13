#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "EnemyState.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
    Idle,
    Attack,
    Dead
};
