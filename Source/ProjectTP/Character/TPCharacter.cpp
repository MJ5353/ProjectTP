#include "TPCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "ProjectTP/Common/TPLog.h"

// ==================================================

ATPCharacter::ATPCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	// 컨트롤러 회전은 카메라에만 쓰고, 캐릭터는 이동 방향으로 돈다
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 기본값이다. 조정은 BP 자식에서 한다
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->bOrientRotationToMovement = true;
	Movement->RotationRate = FRotator(0.f, 500.f, 0.f);
	Movement->JumpZVelocity = 500.f;
	Movement->AirControl = 0.35f;
	Movement->MaxWalkSpeed = 500.f;
	Movement->MinAnalogWalkSpeed = 20.f;
	Movement->BrakingDecelerationWalking = 2000.f;
	Movement->BrakingDecelerationFalling = 1500.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// 메시·AnimBP는 C++이 에셋을 직접 참조하지 않도록 BP 자식에서 지정한다
}

void ATPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInputComponent)
	{
		UE_LOG(LogMj, Error, TEXT("%s: Enhanced Input component not found"), *GetNameSafe(this));
		return;
	}

	// jump
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	// move
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATPCharacter::Move);

	// look
	EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ATPCharacter::Look);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATPCharacter::Look);
}

// Input ------------------------------------------------

void ATPCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	DoMove(MovementVector.X, MovementVector.Y);
}

void ATPCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ATPCharacter::DoMove(const float Right, const float Forward)
{
	if (!GetController())
		return;

	// 카메라가 보는 수평 방향 기준으로 움직인다
	const FRotator YawRotation(0.f, GetController()->GetControlRotation().Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, Forward);
	AddMovementInput(RightDirection, Right);
}

void ATPCharacter::DoLook(const float Yaw, const float Pitch)
{
	if (!GetController())
		return;

	AddControllerYawInput(Yaw);
	AddControllerPitchInput(Pitch);
}

void ATPCharacter::DoJumpStart()
{
	Jump();
}

void ATPCharacter::DoJumpEnd()
{
	StopJumping();
}
