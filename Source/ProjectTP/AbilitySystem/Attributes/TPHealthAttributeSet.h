#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ProjectTP/AbilitySystem/Attributes/TPAttributeSet.h"
#include "TPHealthAttributeSet.generated.h"

// ==================================================

/**
 *  HP 어트리뷰트.
 *  데미지는 GameplayEffect가 메타 어트리뷰트 Damage에 넣고, 여기서 CurrentHP로 옮긴다.
 *  코드에서 CurrentHP를 직접 깎지 않는다.
 */
UCLASS()
class UTPHealthAttributeSet : public UTPAttributeSet
{
	GENERATED_BODY()

public:

	TP_ATTRIBUTE_ACCESSORS(UTPHealthAttributeSet, CurrentHP);
	TP_ATTRIBUTE_ACCESSORS(UTPHealthAttributeSet, MaxHP);
	TP_ATTRIBUTE_ACCESSORS(UTPHealthAttributeSet, Damage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CurrentHP, Category="Health")
	FGameplayAttributeData CurrentHP;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHP, Category="Health")
	FGameplayAttributeData MaxHP;

	/** 들어온 데미지를 잠깐 담는 메타 어트리뷰트. 처리 직후 0으로 돌린다 */
	UPROPERTY(BlueprintReadOnly, Category="Health")
	FGameplayAttributeData Damage;

public:

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	/** CurrentHP를 바꾸고 0이 되면 사망 태그를 붙인다 */
	void SetCurrentHPAndCheckDead(float NewHP);

	UFUNCTION()
	void OnRep_CurrentHP(const FGameplayAttributeData& OldHP);

	UFUNCTION()
	void OnRep_MaxHP(const FGameplayAttributeData& OldMaxHP);
};
