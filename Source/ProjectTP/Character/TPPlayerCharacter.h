#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ProjectTP/Character/TPCharacter.h"
#include "TPPlayerCharacter.generated.h"

class UInputAction;
class UTPAbilityDefinition;
class UTPAbilitySystemComponent;
class UTPAttributeDefinition;

// ==================================================

UCLASS(Abstract)
class PROJECTTP_API ATPPlayerCharacter : public ATPCharacter, public IAbilitySystemInterface
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
	// 입력을 Ability.Key 태그로 ASC에 넘긴다. 서로 막기·끊기는 GA 태그가 판단한다
	UFUNCTION(BlueprintCallable, Category = "MJ - Combat")
	void DoLightAttack();

	UFUNCTION(BlueprintCallable, Category = "MJ - Combat")
	void DoHeavyAttack();

	UFUNCTION(BlueprintCallable, Category = "MJ - Combat")
	void DoDodge();

public:
	// Get ------------------------------------------------
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UTPAbilitySystemComponent* GetTPAbilitySystemComponent() const { return AbilitySystemComponent; }
};
