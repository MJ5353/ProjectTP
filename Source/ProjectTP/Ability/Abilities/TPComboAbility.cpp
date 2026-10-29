#include "TPComboAbility.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Animation/AnimMontage.h"
#include "ProjectTP/Common/TPGameplayTags.h"

// ==================================================

UTPComboAbility::UTPComboAbility(const FObjectInitializer& ObjectInitializer)
{
	// index 정책으로 굴리기 위해서는 per actor여야만 한다
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UTPComboAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || !ComboMontage || ComboSections.IsEmpty())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ResetCombo();

	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, ComboMontage, 1.f, ComboSections[0]);
	MontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnMontageEnded);
	MontageTask->OnBlendOut.AddDynamic(this, &ThisClass::OnMontageEnded);
	MontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnMontageCancelled);
	MontageTask->OnCancelled.AddDynamic(this, &ThisClass::OnMontageCancelled);
	MontageTask->ReadyForActivation();

	// 입력이 없으면 1타에서 끝나도록 에셋의 다음 섹션 연결을 끊기
	MontageSetNextSectionName(ComboSections[0], NAME_None);
}

void UTPComboAbility::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

	// 콤보 창(몽타주 NotifyState가 붙인 태그) 안에 들어온 입력만 다음 타로 넘기기
	const UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC && ASC->HasMatchingGameplayTag(TPGameplayTags::StateTag_ComboWindow))
		QueueNextSection();
}

void UTPComboAbility::ResetCombo()
{
	ComboIndex = 0;
}

void UTPComboAbility::QueueNextSection()
{
	const int32 NextIndex = ComboIndex + 1;
	if (!ComboSections.IsValidIndex(NextIndex))
		return;

	ComboIndex = NextIndex;

	// 다음 틱 NotifyEnd 전에 한 번 더 넘어가지 않도록 이번 창을 바로 닫는다
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		ASC->SetLooseGameplayTagCount(TPGameplayTags::StateTag_ComboWindow, 0);

	// 같은 몽타주 안에서 섹션만 바로 넘기고, 입력이 없으면 그 타에서 끝나게 연결을 끊는다
	MontageJumpToSection(ComboSections[NextIndex]);
	MontageSetNextSectionName(ComboSections[NextIndex], NAME_None);
}

void UTPComboAbility::OnMontageEnded()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UTPComboAbility::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
