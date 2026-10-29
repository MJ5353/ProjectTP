#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h"
#include "TPAnimNotifyState_SendGameplayEvent.generated.h"

// ==================================================

UCLASS(meta = (DisplayName = "Send Gameplay Event State"))
class PROJECTTP_API UTPAnimNotifyState_SendGameplayEvent : public UAnimNotifyState
{
	GENERATED_BODY()

protected:
	// 구간이 시작될 때 보낼 이벤트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MJ - Public", meta = (Categories = "Event"))
	FGameplayTag BeginEventTag;

	// 구간이 끝날 때 보낼 이벤트. 섹션 점프·몽타주 중단으로 구간을 벗어날 때도 보낸다
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MJ - Public", meta = (Categories = "Event"))
	FGameplayTag EndEventTag;

	// ------------------------------------------------

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	void SendEvent(USkeletalMeshComponent* MeshComp, const FGameplayTag& EventTag) const;
};
