#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TPGameplayAbility.generated.h"

// ==================================================

UENUM(BlueprintType)
enum class ETPAbilityActivationPolicy : uint8
{
	Manual,
	OnSpawn,
};

// ==================================================

UCLASS(Abstract)
class PROJECTTP_API UTPGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MJ - Setting")
	ETPAbilityActivationPolicy ActivationPolicy = ETPAbilityActivationPolicy::Manual;
};
