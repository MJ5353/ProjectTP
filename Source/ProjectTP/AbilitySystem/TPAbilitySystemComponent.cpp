#include "TPAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ProjectTP/AbilitySystem/Attributes/TPHealthAttributeSet.h"
#include "ProjectTP/Definition/TPAbilityDefinition.h"
#include "ProjectTP/Definition/TPAttributeDefinition.h"
#include "ProjectTP/AbilitySystem/TPGameplayAbility.h"
#include "ProjectTP/Common/TPGameplayTags.h"

// ==================================================

void UTPAbilitySystemComponent::SetUp(const UTPAbilityDefinition* AbilityDefinition, const UTPAttributeDefinition* AttributeDefinition)
{
	if (!IsOwnerActorAuthoritative())
	{
		return;
	}

	Clear();

	// 어트리뷰트를 먼저 붙여야 OnSpawn 어빌리티가 켜질 때 값을 읽을 수 있다
	if (AttributeDefinition)
	{
		AttributeDefinition->GiveToAbilitySystem(this, &GrantedAttributeSets);
	}
	RestoreHP();

	if (AbilityDefinition)
	{
		AbilityDefinition->GiveToAbilitySystem(this, &GrantedAbilityHandles, GetOwner());
	}

	TryActivateOnSpawnAbilities();
}

void UTPAbilitySystemComponent::Clear()
{
	if (!IsOwnerActorAuthoritative())
	{
		return;
	}

	GrantedAbilityHandles.TakeFromAbilitySystem(this);
	GrantedAttributeSets.TakeFromAbilitySystem(this);
}

void UTPAbilitySystemComponent::RestoreHP()
{
	if (!IsOwnerActorAuthoritative())
	{
		return;
	}

	// 리스폰·리트라이에서 이것만 불러도 살아난 상태가 되도록 사망 태그를 같이 푼다
	SetLooseGameplayTagCount(TPGameplayTags::StateTag_Dead, 0, EGameplayTagReplicationState::TagOnly);

	if (GetSet<UTPHealthAttributeSet>())
	{
		SetNumericAttributeBase(UTPHealthAttributeSet::GetCurrentHpAttribute(), GetNumericAttribute(UTPHealthAttributeSet::GetMaxHpAttribute()));
	}
}

void UTPAbilitySystemComponent::NotifyDamageApplied(AActor* Instigator, float AppliedDamage)
{
	if (!IsOwnerActorAuthoritative() || AppliedDamage <= 0.f)
	{
		return;
	}

	OnDamageApplied.Broadcast(Instigator, AppliedDamage);

	// 죽이는 타격도 보낸다. 받는 쪽에서 State.Dead로 피격 반응과 사망을 나눈다
	AActor* Avatar = GetAvatarActor();
	if (!Avatar)
	{
		return;
	}

	FGameplayEventData HitEventData;
	HitEventData.EventTag = TPGameplayTags::EventTag_Hit;
	HitEventData.Instigator = Instigator;
	HitEventData.Target = Avatar;
	HitEventData.EventMagnitude = AppliedDamage;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Avatar, HitEventData.EventTag, HitEventData);
}

const FGameplayAbilitySpec* UTPAbilitySystemComponent::FindAbilitySpecByKey(const FGameplayTag& AbilityKey) const
{
	if (!AbilityKey.IsValid())
	{
		return nullptr;
	}

	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (AbilitySpec.Handle.IsValid() && AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(AbilityKey))
		{
			return &AbilitySpec;
		}
	}

	return nullptr;
}

bool UTPAbilitySystemComponent::TryActivateAbilityByKey(FGameplayTag AbilityKey)
{
	const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecByKey(AbilityKey);
	return AbilitySpec && TryActivateAbility(AbilitySpec->Handle);
}

bool UTPAbilitySystemComponent::CanActivateAbilityByKey(FGameplayTag AbilityKey) const
{
	const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecByKey(AbilityKey);
	const FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();

	if (!AbilitySpec || !AbilitySpec->Ability || !ActorInfo)
	{
		return false;
	}

	return AbilitySpec->Ability->CanActivateAbility(AbilitySpec->Handle, ActorInfo);
}

bool UTPAbilitySystemComponent::IsAbilityActiveByKey(FGameplayTag AbilityKey) const
{
	const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecByKey(AbilityKey);
	return AbilitySpec && AbilitySpec->IsActive();
}

void UTPAbilitySystemComponent::TryActivateOnSpawnAbilities()
{
	ABILITYLIST_SCOPE_LOCK();
	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		const UTPGameplayAbility* AbilityCDO = Cast<UTPGameplayAbility>(AbilitySpec.Ability);
		if (AbilityCDO && AbilityCDO->ActivationPolicy == ETPAbilityActivationPolicy::OnSpawn && !AbilitySpec.IsActive())
		{
			TryActivateAbility(AbilitySpec.Handle);
		}
	}
}
