#include "TPAbilityDefinition.h"
#include "AbilitySystemComponent.h"
#include "ProjectTP/AbilitySystem/TPGameplayAbility.h"
#include "ProjectTP/AbilitySystem/TPGrantedHandles.h"

void UTPAbilityDefinition::GiveToAbilitySystem(UAbilitySystemComponent* ASC, FTPGrantedAbilityHandles& OutGrantedHandles, UObject* SourceObject) const
{
	check(ASC);

	if (!ASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	for (const TPair<FGameplayTag, TSubclassOf<UTPGameplayAbility>>& Pair : AbilityMap)
	{
		const FGameplayTag& AbilityKey = Pair.Key;
		const TSubclassOf<UTPGameplayAbility>& AbilityClass = Pair.Value;

		if (!AbilityKey.IsValid() || !AbilityClass || OutGrantedHandles.Contains(AbilityKey))
		{
			continue;
		}

		// 키 태그를 Spec의 동적 태그로 붙여 두고, ASC에서 이 태그로 Spec을 찾는다
		FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityKey);

		OutGrantedHandles.Add(AbilityKey, ASC->GiveAbility(AbilitySpec));
	}
}
