#include "TPGrantedAttributeSets.h"
#include "AbilitySystemComponent.h"
#include "ProjectTP/AbilitySystem/Attributes/TPAttributeSet.h"

// ==================================================

void FTPGrantedAttributeSets::AddAttributeSet(UTPAttributeSet* AttributeSet)
{
	if (IsValid(AttributeSet))
		AttributeSets.Add(AttributeSet);
}

void FTPGrantedAttributeSets::TakeFromAbilitySystem(UAbilitySystemComponent* ASC)
{
	if (!ASC || !ASC->IsOwnerActorAuthoritative())
		return;

	for (UTPAttributeSet* Set : AttributeSets)
	{
		if (IsValid(Set))
			ASC->RemoveSpawnedAttribute(Set);
	}

	AttributeSets.Reset();
}
