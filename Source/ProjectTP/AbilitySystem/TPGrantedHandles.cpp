#include "TPGrantedHandles.h"
#include "AbilitySystemComponent.h"
#include "TPAttributeSet.h"

void FTPGrantedAbilityHandles::Add(const FGameplayTag& AbilityKey, const FGameplayAbilitySpecHandle& Handle)
{
	if (!AbilityKey.IsValid() || !Handle.IsValid())
	{
		return;
	}

	if (!ensureMsgf(!KeyToSpecHandle.Contains(AbilityKey), TEXT("Duplicate ability key: %s"), *AbilityKey.ToString()))
	{
		return;
	}

	KeyToSpecHandle.Add(AbilityKey, Handle);
}

bool FTPGrantedAbilityHandles::Contains(const FGameplayTag& AbilityKey) const
{
	return AbilityKey.IsValid() && KeyToSpecHandle.Contains(AbilityKey);
}

void FTPGrantedAbilityHandles::TakeFromAbilitySystem(UAbilitySystemComponent* ASC)
{
	if (!ASC || !ASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	for (const TPair<FGameplayTag, FGameplayAbilitySpecHandle>& Pair : KeyToSpecHandle)
	{
		if (Pair.Value.IsValid())
		{
			ASC->ClearAbility(Pair.Value);
		}
	}

	KeyToSpecHandle.Reset();
}

void FTPGrantedAttributeSets::Add(UTPAttributeSet* AttributeSet)
{
	if (IsValid(AttributeSet))
	{
		AttributeSets.Add(AttributeSet);
	}
}

void FTPGrantedAttributeSets::TakeFromAbilitySystem(UAbilitySystemComponent* ASC)
{
	if (!ASC || !ASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	for (UTPAttributeSet* Set : AttributeSets)
	{
		if (IsValid(Set))
		{
			ASC->RemoveSpawnedAttribute(Set);
		}
	}

	AttributeSets.Reset();
}
