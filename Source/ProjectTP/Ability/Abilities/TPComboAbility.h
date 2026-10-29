#pragma once

#include "CoreMinimal.h"
#include "ProjectTP/Ability/Core/TPGameplayAbility.h"
#include "TPComboAbility.generated.h"

class UAnimMontage;

// ==================================================

UCLASS()
class PROJECTTP_API UTPComboAbility : public UTPGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MJ - Setting")
	TObjectPtr<UAnimMontage> ComboMontage;

	// 콤보 순서대로 ComboMontage의 섹션 이름
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MJ - Setting")
	TArray<FName> ComboSections;

	int32 ComboIndex = 0;

public:
	UTPComboAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	// set
	void ResetCombo();
	void QueueNextSection();

	// ------------------------------------------------

	UFUNCTION()
	void OnMontageEnded();

	UFUNCTION()
	void OnMontageCancelled();
};
