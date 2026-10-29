#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayTagContainer.h"
#include "TPGrantedAbilityHandles.generated.h"

class UAbilitySystemComponent;

// ==================================================

USTRUCT()
struct FTPGrantedAbilityHandles
{
	GENERATED_BODY()

private:
	// 정의 에셋으로 지급한 어빌리티를 정의 교체·정리 때 한 번에 회수하기 위한 기록
	UPROPERTY(Transient)
	TMap<FGameplayTag, FGameplayAbilitySpecHandle> TagToAbilitySpecHandle;

public:
	void AddAbilitySpecHandle(const FGameplayTag& Tag, const FGameplayAbilitySpecHandle& Handle);
	void TakeFromAbilitySystem(UAbilitySystemComponent* ASC);

	bool HasSpecHandle(const FGameplayTag& Tag) const;
};
