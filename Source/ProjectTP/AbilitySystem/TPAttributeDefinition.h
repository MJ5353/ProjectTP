#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Engine/DataAsset.h"
#include "TPAttributeDefinition.generated.h"

class UAbilitySystemComponent;
class UTPAttributeSet;
struct FTPGrantedAttributeSets;

/** 어트리뷰트 초기값 한 줄 */
USTRUCT(BlueprintType)
struct FTPAttributeInitValue
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Attribute")
	FGameplayAttribute Attribute;

	UPROPERTY(EditDefaultsOnly, Category="Attribute")
	float Value = 0.f;
};

/**
 *  캐릭터에 붙일 AttributeSet과 초기값 (DataAsset)
 */
UCLASS()
class UTPAttributeDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category="Attribute")
	TArray<TSubclassOf<UTPAttributeSet>> AttributeSets;

	/**
	 *  배열 순서대로 넣는다.
	 *  CurrentHP는 넣지 않는다: 그 시점의 MaxHP로 잘리므로 MaxHP만 넣고 ASC의 RestoreHP()로 채운다.
	 */
	UPROPERTY(EditDefaultsOnly, Category="Attribute")
	TArray<FTPAttributeInitValue> InitialValues;

	/** 서버에서만 AttributeSet을 만들어 붙이고 초기값을 넣는다 */
	void GiveToAbilitySystem(UAbilitySystemComponent* ASC, FTPGrantedAttributeSets& OutGrantedSets) const;

	/** 이미 붙어 있는 AttributeSet의 값을 초기값으로 되돌린다 */
	void ResetAttributes(UAbilitySystemComponent* ASC) const;
};
