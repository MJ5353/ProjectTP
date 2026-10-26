#pragma once

#include "CoreMinimal.h"
#include "ProjectTPCharacter.h"
#include "TPPlayerCharacter.generated.h"

class UAnimInstance;
class UAnimMontage;
class UInputAction;

/**
 *  플레이어 캐릭터 (Manny + 대검)
 *  1일차 임시 연결: 공격·회피 입력 → 몽타주 재생. 2일차에 GAS 어빌리티로 교체한다.
 */
UCLASS(abstract)
class ATPPlayerCharacter : public AProjectTPCharacter
{
	GENERATED_BODY()

protected:

	/** 약공격 입력 */
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> LightAttackAction;

	/** 강공격 입력 */
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> HeavyAttackAction;

	/** 회피 입력 */
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> DodgeAction;

	/** 약공 3타 몽타주. 섹션마다 1타씩 두고, 다음 타는 입력이 들어올 때만 이어진다 */
	UPROPERTY(EditAnywhere, Category="Combat|Montage")
	TObjectPtr<UAnimMontage> LightAttackMontage;

	/** 약공 몽타주의 섹션 이름 (타수 순서) */
	UPROPERTY(EditAnywhere, Category="Combat|Montage")
	TArray<FName> LightComboSections = { TEXT("Attack1"), TEXT("Attack2"), TEXT("Attack3") };

	/** 강공격 몽타주 */
	UPROPERTY(EditAnywhere, Category="Combat|Montage")
	TObjectPtr<UAnimMontage> HeavyAttackMontage;

	/** 회피 몽타주 (루트 모션) */
	UPROPERTY(EditAnywhere, Category="Combat|Montage")
	TObjectPtr<UAnimMontage> DodgeMontage;

protected:

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:

	/** 약공격. 공격 중에 누르면 현재 타가 끝난 뒤 다음 타로 이어진다 */
	UFUNCTION(BlueprintCallable, Category="Combat")
	void DoLightAttack();

	/** 강공격. 다른 공격·회피 중에는 무시한다 */
	UFUNCTION(BlueprintCallable, Category="Combat")
	void DoHeavyAttack();

	/** 회피. 공격 중에도 바로 끊고 나간다 (캔슬 구간 제한은 2일차) */
	UFUNCTION(BlueprintCallable, Category="Combat")
	void DoDodge();

private:

	/** 메시의 AnimInstance. 없으면 경고 로그를 남기고 nullptr */
	UAnimInstance* GetAnimInstanceOrWarn() const;

	/** 몽타주를 재생하고 모든 섹션의 다음 섹션 연결을 끊는다 */
	void PlayMontageFromSection(UAnimMontage* Montage, FName Section);

	/** 공격이나 회피 몽타주가 재생 중인지 */
	bool IsPlayingActionMontage() const;
};
