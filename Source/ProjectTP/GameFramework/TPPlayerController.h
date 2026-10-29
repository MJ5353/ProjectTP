#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TPPlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;

// ==================================================

UCLASS(Abstract)
class PROJECTTP_API ATPPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	// input
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MJ - Setting")
	TArray<TObjectPtr<UInputMappingContext>> DefaultMappingContexts;

	// 모바일 터치 입력을 쓸 때는 넣지 않는다
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MJ - Setting")
	TArray<TObjectPtr<UInputMappingContext>> MobileExcludedMappingContexts;

	// touch
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MJ - Setting")
	TSubclassOf<UUserWidget> MobileControlsWidgetClass;

	// 모바일 플랫폼이 아니어도 터치 UI를 쓴다
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = "MJ - Setting")
	bool bForceTouchControls = false;

	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> MobileControlsWidget;

	// ------------------------------------------------

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	bool ShouldUseTouchControls() const;
};
