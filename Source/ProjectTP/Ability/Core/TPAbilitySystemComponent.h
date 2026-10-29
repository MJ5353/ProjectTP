#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ProjectTP/Ability/Core/TPGrantedAbilityHandles.h"
#include "ProjectTP/Ability/Core/TPGrantedAttributeSets.h"
#include "TPAbilitySystemComponent.generated.h"

class UTPAbilityDefinition;
class UTPAttributeDefinition;

// ==================================================

// damage
DECLARE_MULTICAST_DELEGATE_TwoParams(FTPDamageAppliedDelegate, AActor* /* Instigator */, float /* AppliedDamage */);

// ------------------------------------------------

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTTP_API UTPAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient)
	FTPGrantedAbilityHandles GrantedAbilityHandles;

	UPROPERTY(Transient)
	FTPGrantedAttributeSets GrantedAttributeSets;

public:
	FTPDamageAppliedDelegate OnDamageApplied;

	// Core ------------------------------------------------
	void SetUp_Server(const UTPAbilityDefinition* AbilityDefinition, const UTPAttributeDefinition* AttributeDefinition);
	void Clear_Server();

protected:
	void TryActivateOnSpawnAbilities();

public:
	// Attribute ------------------------------------------------
	void NotifyDamageApplied(AActor* Instigator, float AppliedDamage);
	void RestoreHp_Server();

	// Ability Tag ------------------------------------------------
	const FGameplayAbilitySpec* FindAbilitySpecByTag(const FGameplayTag& Tag) const;

	UFUNCTION(BlueprintCallable, Category = "MJ - Ability")
	bool TryActivateAbilityByTag(FGameplayTag Tag);

	UFUNCTION(BlueprintPure, Category = "MJ - Ability")
	bool CanActivateAbilityByTag(FGameplayTag Tag) const;

	UFUNCTION(BlueprintPure, Category = "MJ - Ability")
	bool IsAbilityActiveByTag(FGameplayTag Tag) const;
};
