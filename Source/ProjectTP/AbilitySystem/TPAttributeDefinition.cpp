#include "TPAttributeDefinition.h"
#include "AbilitySystemComponent.h"
#include "TPAttributeSet.h"
#include "TPGrantedHandles.h"

void UTPAttributeDefinition::GiveToAbilitySystem(UAbilitySystemComponent* ASC, FTPGrantedAttributeSets& OutGrantedSets) const
{
	check(ASC);

	if (!ASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	for (const TSubclassOf<UTPAttributeSet>& AttributeSetClass : AttributeSets)
	{
		if (!AttributeSetClass || ASC->GetAttributeSet(AttributeSetClass))
		{
			continue;
		}

		UTPAttributeSet* NewSet = NewObject<UTPAttributeSet>(ASC->GetOwner(), AttributeSetClass);
		ASC->AddAttributeSetSubobject(NewSet);
		OutGrantedSets.Add(NewSet);
	}

	ResetAttributes(ASC);
}

void UTPAttributeDefinition::ResetAttributes(UAbilitySystemComponent* ASC) const
{
	if (!ASC || !ASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	for (const FTPAttributeInitValue& InitValue : InitialValues)
	{
		// 붙어 있지 않은 AttributeSet의 값은 건너뛴다
		if (!InitValue.Attribute.IsValid() || !ASC->GetAttributeSet(InitValue.Attribute.GetAttributeSetClass()))
		{
			continue;
		}

		ASC->SetNumericAttributeBase(InitValue.Attribute, InitValue.Value);
	}
}
