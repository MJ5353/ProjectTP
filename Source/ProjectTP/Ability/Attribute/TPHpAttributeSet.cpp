#include "TPHpAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "ProjectTP/Ability/Core/TPAbilitySystemComponent.h"
#include "ProjectTP/Common/TPGameplayTags.h"

// ==================================================

void UTPHpAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if (Attribute == GetCurrentHpAttribute())
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHp());
}

void UTPHpAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const FGameplayAttribute& ModifiedAttribute = Data.EvaluatedData.Attribute;
	if (ModifiedAttribute == GetDamageAttribute())
	{
		const float AppliedDamage = FMath::Max(GetDamage(), 0.f);

		// 다음 데미지와 섞이지 않도록 바로 비운다
		SetDamage(0.f);

		// 이미 죽은 대상에 들어온 데미지는 버린다. 이번 데미지로 죽는 타격은 그대로 알린다
		if (AppliedDamage > 0.f && GetCurrentHp() > 0.f)
		{
			SetAndCheckCurrentHp(FMath::Clamp(GetCurrentHp() - AppliedDamage, 0.f, GetMaxHp()));

			if (UTPAbilitySystemComponent* TPASC = Cast<UTPAbilitySystemComponent>(GetOwningAbilitySystemComponent()))
				TPASC->NotifyDamageApplied(Data.EffectSpec.GetEffectContext().GetOriginalInstigator(), AppliedDamage);
		}
	}
	else if (ModifiedAttribute == GetCurrentHpAttribute())
	{
		SetAndCheckCurrentHp(FMath::Clamp(GetCurrentHp(), 0.f, GetMaxHp()));
	}
	else if (ModifiedAttribute == GetMaxHpAttribute())
	{
		SetMaxHp(FMath::Max(GetMaxHp(), 1.f));
		SetAndCheckCurrentHp(FMath::Clamp(GetCurrentHp(), 0.f, GetMaxHp()));
	}
}

void UTPHpAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UTPHpAttributeSet, CurrentHp, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPHpAttributeSet, MaxHp, COND_None, REPNOTIFY_Always);
}

void UTPHpAttributeSet::SetAndCheckCurrentHp(const float NewHp)
{
	SetCurrentHp(NewHp);

	if (NewHp > 0.f)
		return;

	if (UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
		ASC->SetLooseGameplayTagCount(TPGameplayTags::StateTag_Dead, 1, EGameplayTagReplicationState::TagOnly);
}

void UTPHpAttributeSet::OnRep_CurrentHp(const FGameplayAttributeData& OldHp)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPHpAttributeSet, CurrentHp, OldHp);
}

void UTPHpAttributeSet::OnRep_MaxHp(const FGameplayAttributeData& OldMaxHp)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPHpAttributeSet, MaxHp, OldMaxHp);
}
