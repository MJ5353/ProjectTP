#include "TPAbilityDefinition.h"
#include "AbilitySystemComponent.h"
#include "ProjectTP/Ability/Core/TPGameplayAbility.h"
#include "ProjectTP/Ability/Core/TPGrantedAbilityHandles.h"

// ==================================================

void UTPAbilityDefinition::GiveToAbilitySystem(UAbilitySystemComponent* ASC, FTPGrantedAbilityHandles* OutGrantedHandles, UObject* SourceObject) const
{
	check(ASC);

	if (!OutGrantedHandles || !ASC->IsOwnerActorAuthoritative())
		return;

	for (const TPair<FGameplayTag, TSubclassOf<UTPGameplayAbility>>& Pair : AbilityMap)
	{
		const FGameplayTag& AbilityTag = Pair.Key;
		const TSubclassOf<UTPGameplayAbility>& AbilityClass = Pair.Value;

		if (!AbilityTag.IsValid() || !AbilityClass || OutGrantedHandles->HasSpecHandle(AbilityTag))
			continue;

		// 태그를 Spec의 동적 태그로 붙여 두고, ASC에서 이 태그로 Spec을 찾는다
		FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityTag);

		OutGrantedHandles->AddAbilitySpecHandle(AbilityTag, ASC->GiveAbility(AbilitySpec));
	}
}
