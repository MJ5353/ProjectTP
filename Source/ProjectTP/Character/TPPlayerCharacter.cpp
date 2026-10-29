#include "TPPlayerCharacter.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "EnhancedInputComponent.h"
#include "ProjectTP/Ability/Core/TPAbilitySystemComponent.h"
#include "ProjectTP/Common/TPGameplayTags.h"
#include "ProjectTP/Common/TPLog.h"

// ==================================================

ATPPlayerCharacter::ATPPlayerCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UTPAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

void ATPPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 컨트롤러가 정해진 뒤에 ActorInfo를 잡아야 PlayerController가 채워진다. 빙의될 때마다 갱신한다
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	if (!bAbilitySystemSetUp)
	{
		AbilitySystemComponent->SetUp_Server(AbilityDefinition, AttributeDefinition);
		bAbilitySystemSetUp = true;
	}
}

void ATPPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// BP에서 지정하기 전에는 비어 있으므로 바인딩하지 않고 경고만 남긴다
		auto BindIfSet = [this, EnhancedInputComponent](const UInputAction* Action, const TCHAR* ActionName, void (ATPPlayerCharacter::*Handler)())
		{
			if (Action)
				EnhancedInputComponent->BindAction(Action, ETriggerEvent::Started, this, Handler);
			else
				UE_LOG(LogMj, Warning, TEXT("%s: %s is not set"), *GetName(), ActionName);
		};

		BindIfSet(LightAttackAction, TEXT("LightAttackAction"), &ATPPlayerCharacter::DoLightAttack);
		BindIfSet(HeavyAttackAction, TEXT("HeavyAttackAction"), &ATPPlayerCharacter::DoHeavyAttack);
		BindIfSet(DodgeAction, TEXT("DodgeAction"), &ATPPlayerCharacter::DoDodge);
	}
}

// Action ------------------------------------------------

void ATPPlayerCharacter::DoLightAttack()
{
	if (IsPlayingActionMontage())
		return;

	// 꺼져 있으면 켜고, 켜져 있으면 입력을 넘겨 콤보 어빌리티가 다음 타를 판단한다
	AbilitySystemComponent->AbilityInputPressedByTag(TPGameplayTags::AbilityKeyTag_LightAttack);
}

void ATPPlayerCharacter::DoHeavyAttack()
{
	if (IsPlayingActionMontage())
		return;

	// 약공 중에 막는 건 GA의 State.Attack 태그(Activation Owned·Blocked Tags)가 한다
	AbilitySystemComponent->AbilityInputPressedByTag(TPGameplayTags::AbilityKeyTag_HeavyAttack);
}

void ATPPlayerCharacter::DoDodge()
{
	UAnimInstance* AnimInstance = GetAnimInstanceOrWarn();
	if (!AnimInstance)
		return;

	if (!DodgeMontage)
	{
		UE_LOG(LogMj, Warning, TEXT("%s: DodgeMontage is not set"), *GetName());
		return;
	}

	if (AnimInstance->Montage_IsPlaying(DodgeMontage))
		return;

	// 같은 슬롯의 공격 몽타주는 여기서 끊긴다
	if (AnimInstance->Montage_Play(DodgeMontage) <= 0.f)
	{
		UE_LOG(LogMj, Warning, TEXT("%s: Failed to play DodgeMontage"), *GetName());
		return;
	}

	// 재생에 성공했을 때만 이동 입력 방향으로 돌아서 회피한다
	const FVector InputDirection = GetLastMovementInputVector();
	if (!InputDirection.IsNearlyZero())
		SetActorRotation(FRotator(0.f, InputDirection.Rotation().Yaw, 0.f));
}

// montage

UAnimInstance* ATPPlayerCharacter::GetAnimInstanceOrWarn() const
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
		UE_LOG(LogMj, Warning, TEXT("%s: Mesh has no AnimInstance"), *GetName());

	return AnimInstance;
}

bool ATPPlayerCharacter::IsPlayingActionMontage() const
{
	const UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	// Montage_IsPlaying(nullptr)은 아무 몽타주나 재생 중이면 true라서 null을 먼저 거른다
	return AnimInstance && DodgeMontage && AnimInstance->Montage_IsPlaying(DodgeMontage);
}

// Get ------------------------------------------------

UAbilitySystemComponent* ATPPlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
