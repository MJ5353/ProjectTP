#include "TPDodgeAbility.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Pawn.h"
#include "ProjectTP/Common/TPGameplayTags.h"

// ==================================================

bool UTPDodgeAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
		return false;

	const UAbilitySystemComponent* ASC = ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr;
	if (!ASC)
		return false;

	// 공격 중에는 몽타주의 캔슬 창 안에서만 회피로 끊을 수 있다
	if (ASC->HasMatchingGameplayTag(TPGameplayTags::StateTag_Attack) && !ASC->HasMatchingGameplayTag(TPGameplayTags::StateTag_CancelWindow))
		return false;

	return true;
}

void UTPDodgeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// 활성 조건은 이미 통과했으니 재생 전에 돌린다. 재생이 실패해도 방향만 바뀐 것이라 되돌리지 않는다
	APawn* Pawn = ActorInfo ? Cast<APawn>(ActorInfo->AvatarActor.Get()) : nullptr;
	if (Pawn)
	{
		const FVector InputDirection = Pawn->GetLastMovementInputVector();
		if (!InputDirection.IsNearlyZero())
			Pawn->SetActorRotation(FRotator(0.f, InputDirection.Rotation().Yaw, 0.f));
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
