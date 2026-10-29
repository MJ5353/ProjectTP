#include "TPAnimNotifyState_AddGameplayTag.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Components/SkeletalMeshComponent.h"

// ==================================================

void UTPAnimNotifyState_AddGameplayTag::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	SetTagCount(MeshComp, 1);
}

void UTPAnimNotifyState_AddGameplayTag::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	SetTagCount(MeshComp, 0);
}

void UTPAnimNotifyState_AddGameplayTag::SetTagCount(USkeletalMeshComponent* MeshComp, const int32 NewCount) const
{
	if (!MeshComp || !Tag.IsValid())
		return;

	// 몽타주 에디터 미리보기 액터에는 ASC가 없다
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MeshComp->GetOwner());
	if (!ASC)
		return;

	// 개수를 더하고 빼지 않고 0/1로 정해서, 어빌리티가 먼저 닫아도 NotifyEnd가 경고 없이 지나간다
	ASC->SetLooseGameplayTagCount(Tag, NewCount);
}
