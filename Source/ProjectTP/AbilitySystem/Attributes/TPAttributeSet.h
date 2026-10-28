#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "TPAttributeSet.generated.h"

/**
 *  어트리뷰트 접근 함수 생성
 *  예: GetHealthAttribute(), GetHealth(), SetHealth(), InitHealth()
 */
#define TP_ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 *  프로젝트 AttributeSet의 베이스. 값이 바뀔 때마다 로그를 남긴다 (Verbose)
 */
UCLASS(Abstract)
class UTPAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const override;
};
