#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h"
#include "TPAnimNotifyState_AddGameplayTag.generated.h"

// ==================================================

UCLASS(meta = (DisplayName = "Add Gameplay Tag State"))
class PROJECTTP_API UTPAnimNotifyState_AddGameplayTag : public UAnimNotifyState
{
	GENERATED_BODY()

protected:
	// 구간 동안 오너 ASC에 붙여 두는 루즈 태그. 같은 태그의 구간을 겹쳐 두지 않는다
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MJ - Public", meta = (Categories = "State.Window"))
	FGameplayTag Tag;

	// ------------------------------------------------

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	void SetTagCount(USkeletalMeshComponent* MeshComp, int32 NewCount) const;
};
