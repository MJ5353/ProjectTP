#include "TPAttributeSet.h"
#include "ProjectTP/Common/TPLog.h"

namespace
{
	void LogAttributeChange(const UTPAttributeSet* AttributeSet, const TCHAR* ValueType, const FGameplayAttribute& Attribute, float OldValue, float NewValue)
	{
#if !UE_BUILD_SHIPPING
		UE_LOG(LogMj, Verbose, TEXT("[%s][%s] %s: %.2f -> %.2f"), *GetNameSafe(AttributeSet->GetOwningActor()), ValueType, *Attribute.GetName(), OldValue, NewValue);
#endif
	}
}

void UTPAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	LogAttributeChange(this, TEXT("Current"), Attribute, OldValue, NewValue);
}

void UTPAttributeSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
	LogAttributeChange(this, TEXT("Base"), Attribute, OldValue, NewValue);
}
