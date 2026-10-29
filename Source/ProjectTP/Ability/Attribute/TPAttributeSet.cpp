#include "TPAttributeSet.h"

DEFINE_LOG_CATEGORY_STATIC(LogTPAttribute, Log, All);

// ==================================================

namespace
{
	void LogAttributeChange(const UTPAttributeSet* AttributeSet, const TCHAR* ValueType, const FGameplayAttribute& Attribute, float OldValue, float NewValue)
	{
#if !UE_BUILD_SHIPPING
		UE_LOG(LogTPAttribute, Verbose, TEXT("[%s][%s] %s: %.2f -> %.2f"), *GetNameSafe(AttributeSet->GetOwningActor()), ValueType, *Attribute.GetName(), OldValue, NewValue);
#endif
	}
}

void UTPAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, const float OldValue, const float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	LogAttributeChange(this, TEXT("Current"), Attribute, OldValue, NewValue);
}

void UTPAttributeSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, const float OldValue, const float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
	LogAttributeChange(this, TEXT("Base"), Attribute, OldValue, NewValue);
}
