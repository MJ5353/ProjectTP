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
		EnhancedInputComponent->BindAction(LightAttackAction, ETriggerEvent::Started, this, &ATPPlayerCharacter::DoLightAttack);
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Started, this, &ATPPlayerCharacter::DoHeavyAttack);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &ATPPlayerCharacter::DoDodge);
	}
}

void ATPPlayerCharacter::DoLightAttack()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance || !LightAttackMontage || LightComboSections.IsEmpty())
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

	PlayMontageFromSection(LightAttackMontage, LightComboSections[0]);
}

void ATPPlayerCharacter::DoHeavyAttack()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance || !HeavyAttackMontage)
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
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance || !DodgeMontage)
	{
		UE_LOG(LogProjectTP, Warning, TEXT("%s: DodgeMontage is not set"), *GetName());
		return;
	}

	if (AnimInstance->Montage_IsPlaying(DodgeMontage))
	{
		return;
	}

	// 이동 입력이 있으면 그 방향으로 돌아서 회피한다
	const FVector InputDirection = GetLastMovementInputVector();
	if (!InputDirection.IsNearlyZero())
	{
		SetActorRotation(FRotator(0.f, InputDirection.Rotation().Yaw, 0.f));
	}

	// 같은 슬롯의 공격 몽타주는 여기서 끊긴다
	AnimInstance->Montage_Play(DodgeMontage);
}

void ATPPlayerCharacter::PlayMontageFromSection(UAnimMontage* Montage, FName Section)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
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
