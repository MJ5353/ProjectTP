#pragma once

#include "CoreMinimal.h"
#include "TPGrantedAttributeSets.generated.h"

class UAbilitySystemComponent;
class UTPAttributeSet;

// ==================================================

USTRUCT()
struct FTPGrantedAttributeSets
{
	GENERATED_BODY()

private:
	UPROPERTY(Transient)
	TArray<TObjectPtr<UTPAttributeSet>> AttributeSets;

public:
	void AddAttributeSet(UTPAttributeSet* AttributeSet);
	void TakeFromAbilitySystem(UAbilitySystemComponent* ASC);
};
