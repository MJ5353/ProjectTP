#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ProjectTP/Ability/Attribute/TPAttributeSet.h"
#include "TPHpAttributeSet.generated.h"

// ==================================================

UCLASS()
class PROJECTTP_API UTPHpAttributeSet : public UTPAttributeSet
{
	GENERATED_BODY()

public:
	TP_ATTRIBUTE_ACCESSORS(UTPHpAttributeSet, CurrentHp);
	TP_ATTRIBUTE_ACCESSORS(UTPHpAttributeSet, MaxHp);
	TP_ATTRIBUTE_ACCESSORS(UTPHpAttributeSet, Damage);

	// ------------------------------------------------

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentHp, Category = "MJ - Replicate")
	FGameplayAttributeData CurrentHp;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHp, Category = "MJ - Replicate")
	FGameplayAttributeData MaxHp;

	UPROPERTY(BlueprintReadOnly, Category = "MJ - Runtime")
	FGameplayAttributeData Damage;

	// ------------------------------------------------

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	void SetAndCheckCurrentHp(float NewHp);

	UFUNCTION()
	void OnRep_CurrentHp(const FGameplayAttributeData& OldHp);

	UFUNCTION()
	void OnRep_MaxHp(const FGameplayAttributeData& OldMaxHp);
};
