#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TPGameplayAbility.generated.h"

/** 어빌리티를 언제 켜는지 */
UENUM(BlueprintType)
enum class ETPAbilityActivationPolicy : uint8
{
	/** 입력 등으로 직접 켠다 */
	Manual,

	/** 지급되자마자 켠다 (패시브) */
	OnSpawn,
};

/**
 *  프로젝트 GameplayAbility의 베이스. 공격·회피 같은 행동은 이 클래스를 상속해서 만든다
 */
UCLASS(Abstract)
class UTPGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability")
	ETPAbilityActivationPolicy ActivationPolicy = ETPAbilityActivationPolicy::Manual;
};
