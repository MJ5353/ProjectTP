#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ProjectTP/ProjectTPCharacter.h"
#include "TPPlayerCharacter.generated.h"

class UAnimInstance;
class UAnimMontage;
class UInputAction;
class UTPAbilityDefinition;
class UTPAbilitySystemComponent;
class UTPAttributeDefinition;

// ==================================================

UCLASS(Abstract)
class PROJECTTP_API ATPPlayerCharacter : public AProjectTPCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

protected:
	// 싱글 플레이 데모라 ASC를 PlayerState가 아니라 캐릭터가 직접 가진다
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MJ - Component")
	TObjectPtr<UTPAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MJ - Setting")
	TObjectPtr<UTPAbilityDefinition> AbilityDefinition;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MJ - Setting")
	TObjectPtr<UTPAttributeDefinition> AttributeDefinition;

	// input
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MJ - Setting")
	TObjectPtr<UInputAction> LightAttackAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MJ - Setting")
	TObjectPtr<UInputAction> HeavyAttackAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MJ - Setting")
	TObjectPtr<UInputAction> DodgeAction;

	// montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MJ - Setting")
	TObjectPtr<UAnimMontage> LightAttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MJ - Setting")
	TArray<FName> LightComboSections = { TEXT("Attack1"), TEXT("Attack2"), TEXT("Attack3") };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MJ - Setting")
	TObjectPtr<UAnimMontage> HeavyAttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MJ - Setting")
	TObjectPtr<UAnimMontage> DodgeMontage;

	// 다시 빙의돼도 HP·진행 중인 어빌리티를 초기화하지 않도록 지급은 한 번만 한다
	bool bAbilitySystemSetUp = false;

	// ------------------------------------------------

public:
	ATPPlayerCharacter();

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:
	// Action ------------------------------------------------
	// 임시: 입력 → 몽타주 재생. GAS 어빌리티로 옮길 예정
	UFUNCTION(BlueprintCallable, Category = "MJ - Combat")
	void DoLightAttack();

	UFUNCTION(BlueprintCallable, Category = "MJ - Combat")
	void DoHeavyAttack();

	UFUNCTION(BlueprintCallable, Category = "MJ - Combat")
	void DoDodge();

private:
	// montage
	UAnimInstance* GetAnimInstanceOrWarn() const;
	void PlayMontageFromSection(UAnimInstance* AnimInstance, UAnimMontage* Montage, FName Section);
	bool IsPlayingActionMontage() const;

public:
	// Get ------------------------------------------------
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UTPAbilitySystemComponent* GetTPAbilitySystemComponent() const { return AbilitySystemComponent; }
};
