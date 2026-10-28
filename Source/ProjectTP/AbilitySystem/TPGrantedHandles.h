#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayTagContainer.h"
#include "TPGrantedHandles.generated.h"

class UAbilitySystemComponent;
class UTPAttributeSet;

/**
 *  정의 에셋으로 지급한 어빌리티 기록. 정의를 바꾸거나 정리할 때 한 번에 회수한다
 */
USTRUCT()
struct FTPGrantedAbilityHandles
{
	GENERATED_BODY()

private:

	/** 어빌리티 키 태그 → 지급한 Spec 핸들 */
	UPROPERTY(Transient)
	TMap<FGameplayTag, FGameplayAbilitySpecHandle> KeyToSpecHandle;

public:

	void Add(const FGameplayTag& AbilityKey, const FGameplayAbilitySpecHandle& Handle);
	bool Contains(const FGameplayTag& AbilityKey) const;
	void TakeFromAbilitySystem(UAbilitySystemComponent* ASC);
};

/**
 *  정의 에셋으로 만들어 붙인 AttributeSet 기록
 */
USTRUCT()
struct FTPGrantedAttributeSets
{
	GENERATED_BODY()

private:

	UPROPERTY(Transient)
	TArray<TObjectPtr<UTPAttributeSet>> AttributeSets;

public:

	void Add(UTPAttributeSet* AttributeSet);
	void TakeFromAbilitySystem(UAbilitySystemComponent* ASC);
};
