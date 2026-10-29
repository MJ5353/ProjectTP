#include "TPAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ProjectTP/Ability/Attribute/TPHpAttributeSet.h"
#include "ProjectTP/Definition/GameplayAbility/TPAbilityDefinition.h"
#include "ProjectTP/Definition/GameplayAbility/TPAttributeDefinition.h"
#include "ProjectTP/Ability/Core/TPGameplayAbility.h"
#include "ProjectTP/Common/TPGameplayTags.h"

// ==================================================

// Core ------------------------------------------------

void UTPAbilitySystemComponent::SetUp_Server(const UTPAbilityDefinition* AbilityDefinition, const UTPAttributeDefinition* AttributeDefinition)
{
	if (!IsOwnerActorAuthoritative())
		return;

	Clear_Server();

	// 어트리뷰트를 먼저 붙여야 OnSpawn 어빌리티가 켜질 때 값을 읽을 수 있다
	if (AttributeDefinition)
		AttributeDefinition->GiveToAbilitySystem(this, &GrantedAttributeSets);

	RestoreHp_Server();

	if (AbilityDefinition)
		AbilityDefinition->GiveToAbilitySystem(this, &GrantedAbilityHandles, GetOwner());

	TryActivateOnSpawnAbilities();
}

void UTPAbilitySystemComponent::Clear_Server()
{
	if (!IsOwnerActorAuthoritative())
		return;

	GrantedAbilityHandles.TakeFromAbilitySystem(this);
	GrantedAttributeSets.TakeFromAbilitySystem(this);
}

void UTPAbilitySystemComponent::TryActivateOnSpawnAbilities()
{
	ABILITYLIST_SCOPE_LOCK();
	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		const UTPGameplayAbility* AbilityCDO = Cast<UTPGameplayAbility>(AbilitySpec.Ability);
		if (AbilityCDO && AbilityCDO->ActivationPolicy == ETPAbilityActivationPolicy::OnSpawn && !AbilitySpec.IsActive())
			TryActivateAbility(AbilitySpec.Handle);
	}
}

// Attribute ------------------------------------------------

void UTPAbilitySystemComponent::NotifyDamageApplied(AActor* Instigator, const float AppliedDamage)
{
	if (!IsOwnerActorAuthoritative() || AppliedDamage <= 0.f)
		return;

	OnDamageApplied.Broadcast(Instigator, AppliedDamage);

	// 죽이는 타격도 보낸다. 받는 쪽에서 State.Dead로 피격 반응과 사망을 나눈다
	AActor* Avatar = GetAvatarActor();
	if (!Avatar)
		return;

	FGameplayEventData HitEventData;
	HitEventData.EventTag = TPGameplayTags::EventTag_Hit;
	HitEventData.Instigator = Instigator;
	HitEventData.Target = Avatar;
	HitEventData.EventMagnitude = AppliedDamage;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Avatar, HitEventData.EventTag, HitEventData);
}

void UTPAbilitySystemComponent::RestoreHp_Server()
{
	if (!IsOwnerActorAuthoritative())
		return;

	// 리스폰·리트라이에서 이것만 불러도 살아난 상태가 되도록 사망 태그를 같이 푼다
	SetLooseGameplayTagCount(TPGameplayTags::StateTag_Dead, 0, EGameplayTagReplicationState::TagOnly);

	// 초기화 전용이라 값을 직접 쓴다. 전투 중 회복은 GameplayEffect로만 한다
	if (GetSet<UTPHpAttributeSet>())
		SetNumericAttributeBase(UTPHpAttributeSet::GetCurrentHpAttribute(), GetNumericAttribute(UTPHpAttributeSet::GetMaxHpAttribute()));
}

// Ability Tag ------------------------------------------------

const FGameplayAbilitySpec* UTPAbilitySystemComponent::FindAbilitySpecByTag(const FGameplayTag& Tag) const
{
	if (!Tag.IsValid())
		return nullptr;

	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (AbilitySpec.Handle.IsValid() && AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(Tag))
			return &AbilitySpec;
	}

	return nullptr;
}

void UTPAbilitySystemComponent::AbilityInputPressedByTag(const FGameplayTag& Tag)
{
	const FGameplayAbilitySpec* FoundSpec = FindAbilitySpecByTag(Tag);
	if (!FoundSpec)
		return;

	FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(FoundSpec->Handle);
	if (!AbilitySpec)
		return;

	// 이미 켜져 있으면 입력만 전달하고(콤보 다음 타 등), 아니면 켠다
	if (AbilitySpec->IsActive())
		AbilitySpecInputPressed(*AbilitySpec);
	else
		TryActivateAbility(AbilitySpec->Handle);
}

bool UTPAbilitySystemComponent::TryActivateAbilityByTag(const FGameplayTag Tag)
{
	const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecByTag(Tag);
	return AbilitySpec && TryActivateAbility(AbilitySpec->Handle);
}

bool UTPAbilitySystemComponent::CanActivateAbilityByTag(const FGameplayTag Tag) const
{
	const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecByTag(Tag);
	const FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();

	if (!AbilitySpec || !AbilitySpec->Ability || !ActorInfo)
		return false;

	return AbilitySpec->Ability->CanActivateAbility(AbilitySpec->Handle, ActorInfo);
}

bool UTPAbilitySystemComponent::IsAbilityActiveByTag(const FGameplayTag Tag) const
{
	const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecByTag(Tag);
	return AbilitySpec && AbilitySpec->IsActive();
}
