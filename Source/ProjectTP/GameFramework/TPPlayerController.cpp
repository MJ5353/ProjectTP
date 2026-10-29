#include "TPPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "ProjectTP/Common/TPLog.h"
#include "Widgets/Input/SVirtualJoystick.h"

// ==================================================

void ATPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 터치 UI는 로컬 플레이어 컨트롤러에만 띄운다
	if (!ShouldUseTouchControls() || !IsLocalPlayerController())
		return;

	MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);
	if (!MobileControlsWidget)
	{
		UE_LOG(LogMj, Error, TEXT("%s: Could not spawn mobile controls widget"), *GetName());
		return;
	}

	MobileControlsWidget->AddToPlayerScreen(0);
}

void ATPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// IMC는 로컬 플레이어 컨트롤러에만 넣는다
	if (!IsLocalPlayerController())
		return;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!Subsystem)
		return;

	for (UInputMappingContext* Context : DefaultMappingContexts)
		Subsystem->AddMappingContext(Context, 0);

	if (ShouldUseTouchControls())
		return;

	for (UInputMappingContext* Context : MobileExcludedMappingContexts)
		Subsystem->AddMappingContext(Context, 0);
}

bool ATPPlayerController::ShouldUseTouchControls() const
{
	return SVirtualJoystick::ShouldDisplayTouchInterface() || bForceTouchControls;
}
