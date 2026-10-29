#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "TPAnimNotify_SendGameplayEvent.generated.h"

// ==================================================

UCLASS(meta = (DisplayName = "Send Gameplay Event"))
class PROJECTTP_API UTPAnimNotify_SendGameplayEvent : public UAnimNotify
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MJ - Public", meta = (Categories = "Event"))
	FGameplayTag EventTag;
	
	// ------------------------------------------------
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
