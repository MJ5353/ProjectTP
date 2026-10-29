#include "TPAttributeDefinition.h"
#include "AbilitySystemComponent.h"
#include "ProjectTP/AbilitySystem/Attributes/TPAttributeSet.h"
#include "ProjectTP/AbilitySystem/TPGrantedAttributeSets.h"

// ==================================================

void UTPAttributeDefinition::GiveToAbilitySystem(UAbilitySystemComponent* ASC, FTPGrantedAttributeSets* OutGrantedAttributeSets) const
{
	check(ASC);

	if (!ASC->IsOwnerActorAuthoritative())
		return;

	for (const TSubclassOf<UTPAttributeSet>& AttributeSetClass : AttributeSets)
	{
		if (!AttributeSetClass || ASC->GetAttributeSet(AttributeSetClass))
			continue;

		UTPAttributeSet* NewSet = NewObject<UTPAttributeSet>(ASC->GetOwner(), AttributeSetClass);
		ASC->AddAttributeSetSubobject(NewSet);

		if (OutGrantedAttributeSets)
			OutGrantedAttributeSets->AddAttributeSet(NewSet);
	}

	ResetAttributes(ASC);
}

void UTPAttributeDefinition::ResetAttributes(UAbilitySystemComponent* ASC) const
{
	if (!ASC || !ASC->IsOwnerActorAuthoritative())
		return;

	// 초기화 전용이라 값을 직접 쓴다. 전투 중 변경은 GameplayEffect로만 한다
	for (const FTPAttributeInitValue& InitValue : InitialValues)
	{
		// 붙어 있지 않은 AttributeSet의 값은 건너뛴다
		if (!InitValue.Attribute.IsValid() || !ASC->GetAttributeSet(InitValue.Attribute.GetAttributeSetClass()))
			continue;

		ASC->SetNumericAttributeBase(InitValue.Attribute, InitValue.Value);
	}
}
