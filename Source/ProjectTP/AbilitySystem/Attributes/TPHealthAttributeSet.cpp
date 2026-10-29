#include "TPHealthAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "ProjectTP/AbilitySystem/TPAbilitySystemComponent.h"
#include "ProjectTP/Common/TPGameplayTags.h"

// ==================================================

void UTPHealthAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if (Attribute == GetCurrentHPAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHP());
	}
}

void UTPHealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const FGameplayAttribute& ModifiedAttribute = Data.EvaluatedData.Attribute;
	if (ModifiedAttribute == GetDamageAttribute())
	{
		const float AppliedDamage = FMath::Max(GetDamage(), 0.f);

		// 다음 데미지와 섞이지 않도록 바로 비운다
		SetDamage(0.f);

		// 이미 죽은 대상에 들어온 데미지는 버린다. 이번 데미지로 죽는 타격은 그대로 알린다
		if (AppliedDamage > 0.f && GetCurrentHP() > 0.f)
		{
			SetCurrentHPAndCheckDead(FMath::Clamp(GetCurrentHP() - AppliedDamage, 0.f, GetMaxHP()));

			if (UTPAbilitySystemComponent* TPASC = Cast<UTPAbilitySystemComponent>(GetOwningAbilitySystemComponent()))
			{
				TPASC->NotifyDamageApplied(Data.EffectSpec.GetEffectContext().GetOriginalInstigator(), AppliedDamage);
			}
		}
	}
	else if (ModifiedAttribute == GetCurrentHPAttribute())
	{
		SetCurrentHPAndCheckDead(FMath::Clamp(GetCurrentHP(), 0.f, GetMaxHP()));
	}
	else if (ModifiedAttribute == GetMaxHPAttribute())
	{
		SetMaxHP(FMath::Max(GetMaxHP(), 1.f));
		SetCurrentHPAndCheckDead(FMath::Clamp(GetCurrentHP(), 0.f, GetMaxHP()));
	}
}

void UTPHealthAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UTPHealthAttributeSet, CurrentHP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPHealthAttributeSet, MaxHP, COND_None, REPNOTIFY_Always);
}

void UTPHealthAttributeSet::SetCurrentHPAndCheckDead(float NewHP)
{
	SetCurrentHP(NewHP);

	if (NewHP > 0.f)
	{
		return;
	}

	if (UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
	{
		ASC->SetLooseGameplayTagCount(TPGameplayTags::StateTag_Dead, 1, EGameplayTagReplicationState::TagOnly);
	}
}

void UTPHealthAttributeSet::OnRep_CurrentHP(const FGameplayAttributeData& OldHP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPHealthAttributeSet, CurrentHP, OldHP);
}

void UTPHealthAttributeSet::OnRep_MaxHP(const FGameplayAttributeData& OldMaxHP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPHealthAttributeSet, MaxHP, OldMaxHP);
}
