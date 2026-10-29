#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Engine/DataAsset.h"
#include "TPAttributeDefinition.generated.h"

class UAbilitySystemComponent;
class UTPAttributeSet;
struct FTPGrantedAttributeSets;

// ==================================================

USTRUCT(BlueprintType)
struct FTPAttributeInitValue
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MJ - Setting")
	FGameplayAttribute Attribute;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MJ - Setting")
	float Value = 0.f;
};

// ==================================================

UCLASS()
class PROJECTTP_API UTPAttributeDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MJ - Setting")
	TArray<TSubclassOf<UTPAttributeSet>> AttributeSets;

	// 배열 순서대로 넣는다. CurrentHp는 그 시점의 MaxHp로 잘리므로 넣지 말고 MaxHp만 넣는다 (CurrentHp는 ASC의 RestoreHp_Server가 채움)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MJ - Setting")
	TArray<FTPAttributeInitValue> InitialValues;

	// ------------------------------------------------

	void GiveToAbilitySystem(UAbilitySystemComponent* ASC, FTPGrantedAttributeSets* OutGrantedAttributeSets) const;
	void ResetAttributes(UAbilitySystemComponent* ASC) const;
};
