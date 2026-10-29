#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPCharacter.generated.h"

class UCameraComponent;
class UInputAction;
class USpringArmComponent;
struct FInputActionValue;

// ==================================================

UCLASS(Abstract)
class PROJECTTP_API ATPCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MJ - Component")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MJ - Component")
	TObjectPtr<UCameraComponent> FollowCamera;

	// input
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MJ - Setting")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MJ - Setting")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MJ - Setting")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MJ - Setting")
	TObjectPtr<UInputAction> MouseLookAction;

	// ------------------------------------------------

public:
	ATPCharacter();

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Input ------------------------------------------------
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

public:
	// 입력 액션 말고 UI(모바일 터치 등)에서도 부를 수 있게 BP에 연다
	UFUNCTION(BlueprintCallable, Category = "MJ - Input")
	virtual void DoMove(float Right, float Forward);

	UFUNCTION(BlueprintCallable, Category = "MJ - Input")
	virtual void DoLook(float Yaw, float Pitch);

	UFUNCTION(BlueprintCallable, Category = "MJ - Input")
	virtual void DoJumpStart();

	UFUNCTION(BlueprintCallable, Category = "MJ - Input")
	virtual void DoJumpEnd();

	// Get ------------------------------------------------
	USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
