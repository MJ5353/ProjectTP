#include "TPPlayerCharacter.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "EnhancedInputComponent.h"
#include "ProjectTP.h"

void ATPPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// BP에서 지정하기 전에는 비어 있으므로 바인딩하지 않고 경고만 남긴다
		auto BindIfSet = [this, EnhancedInputComponent](const UInputAction* Action, const TCHAR* ActionName, void (ATPPlayerCharacter::*Handler)())
		{
			if (Action)
			{
				EnhancedInputComponent->BindAction(Action, ETriggerEvent::Started, this, Handler);
			}
			else
			{
				UE_LOG(LogProjectTP, Warning, TEXT("%s: %s is not set"), *GetName(), ActionName);
			}
		};

		BindIfSet(LightAttackAction, TEXT("LightAttackAction"), &ATPPlayerCharacter::DoLightAttack);
		BindIfSet(HeavyAttackAction, TEXT("HeavyAttackAction"), &ATPPlayerCharacter::DoHeavyAttack);
		BindIfSet(DodgeAction, TEXT("DodgeAction"), &ATPPlayerCharacter::DoDodge);
	}
}

void ATPPlayerCharacter::DoLightAttack()
{
	UAnimInstance* AnimInstance = GetAnimInstanceOrWarn();
	if (!AnimInstance)
	{
		return;
	}

	if (!LightAttackMontage || LightComboSections.IsEmpty())
	{
		UE_LOG(LogProjectTP, Warning, TEXT("%s: LightAttackMontage or LightComboSections is not set"), *GetName());
		return;
	}

	// 약공 중이면 현재 타 다음에 이어질 섹션만 예약한다
	if (AnimInstance->Montage_IsPlaying(LightAttackMontage))
	{
		const FName CurrentSection = AnimInstance->Montage_GetCurrentSection(LightAttackMontage);
		const int32 Index = LightComboSections.IndexOfByKey(CurrentSection);
		if (Index != INDEX_NONE && LightComboSections.IsValidIndex(Index + 1))
		{
			AnimInstance->Montage_SetNextSection(CurrentSection, LightComboSections[Index + 1], LightAttackMontage);
		}
		return;
	}

	if (IsPlayingActionMontage())
	{
		return;
	}

	PlayMontageFromSection(AnimInstance, LightAttackMontage, LightComboSections[0]);
}

void ATPPlayerCharacter::DoHeavyAttack()
{
	UAnimInstance* AnimInstance = GetAnimInstanceOrWarn();
	if (!AnimInstance)
	{
		return;
	}

	if (!HeavyAttackMontage)
	{
		UE_LOG(LogProjectTP, Warning, TEXT("%s: HeavyAttackMontage is not set"), *GetName());
		return;
	}

	if (IsPlayingActionMontage())
	{
		return;
	}

	AnimInstance->Montage_Play(HeavyAttackMontage);
}

void ATPPlayerCharacter::DoDodge()
{
	UAnimInstance* AnimInstance = GetAnimInstanceOrWarn();
	if (!AnimInstance)
	{
		return;
	}

	if (!DodgeMontage)
	{
		UE_LOG(LogProjectTP, Warning, TEXT("%s: DodgeMontage is not set"), *GetName());
		return;
	}

	if (AnimInstance->Montage_IsPlaying(DodgeMontage))
	{
		return;
	}

	// 같은 슬롯의 공격 몽타주는 여기서 끊긴다
	if (AnimInstance->Montage_Play(DodgeMontage) <= 0.f)
	{
		UE_LOG(LogProjectTP, Warning, TEXT("%s: Failed to play DodgeMontage"), *GetName());
		return;
	}

	// 재생에 성공했을 때만 이동 입력 방향으로 돌아서 회피한다
	const FVector InputDirection = GetLastMovementInputVector();
	if (!InputDirection.IsNearlyZero())
	{
		SetActorRotation(FRotator(0.f, InputDirection.Rotation().Yaw, 0.f));
	}
}

UAnimInstance* ATPPlayerCharacter::GetAnimInstanceOrWarn() const
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		UE_LOG(LogProjectTP, Warning, TEXT("%s: Mesh has no AnimInstance"), *GetName());
	}
	return AnimInstance;
}

void ATPPlayerCharacter::PlayMontageFromSection(UAnimInstance* AnimInstance, UAnimMontage* Montage, FName Section)
{
	check(AnimInstance && Montage);

	if (AnimInstance->Montage_Play(Montage) <= 0.f)
	{
		return;
	}

	AnimInstance->Montage_JumpToSection(Section, Montage);

	// 에셋의 섹션 연결과 상관없이 입력이 없으면 현재 타에서 끝나게 한다
	for (const FCompositeSection& CompositeSection : Montage->CompositeSections)
	{
		AnimInstance->Montage_SetNextSection(CompositeSection.SectionName, NAME_None, Montage);
	}
}

bool ATPPlayerCharacter::IsPlayingActionMontage() const
{
	const UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		return false;
	}

	// Montage_IsPlaying(nullptr)은 아무 몽타주나 재생 중이면 true라서 null을 먼저 거른다
	for (const UAnimMontage* Montage : { LightAttackMontage.Get(), HeavyAttackMontage.Get(), DodgeMontage.Get() })
	{
		if (Montage && AnimInstance->Montage_IsPlaying(Montage))
		{
			return true;
		}
	}
	return false;
}
