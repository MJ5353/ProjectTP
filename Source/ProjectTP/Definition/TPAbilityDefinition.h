#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "TPAbilityDefinition.generated.h"

class UAbilitySystemComponent;
class UTPGameplayAbility;
struct FTPGrantedAbilityHandles;

// ==================================================

UCLASS()
class PROJECTTP_API UTPAbilityDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MJ - Setting", meta = (Categories = "Ability.Key"))
	TMap<FGameplayTag, TSubclassOf<UTPGameplayAbility>> AbilityMap;

	// ------------------------------------------------

	void GiveToAbilitySystem(UAbilitySystemComponent* ASC, FTPGrantedAbilityHandles* OutGrantedHandles, UObject* SourceObject = nullptr) const;
};
