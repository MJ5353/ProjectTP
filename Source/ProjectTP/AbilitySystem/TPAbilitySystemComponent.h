#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ProjectTP/AbilitySystem/TPGrantedHandles.h"
#include "TPAbilitySystemComponent.generated.h"

class UTPAbilityDefinition;
class UTPAttributeDefinition;

DECLARE_MULTICAST_DELEGATE_TwoParams(FTPDamageAppliedDelegate, AActor* /* Instigator */, float /* AppliedDamage */);

/**
 *  프로젝트 AbilitySystemComponent.
 *  정의 에셋으로 어빌리티·어트리뷰트를 지급하고, 어빌리티 키 태그로 어빌리티를 찾아 켠다.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UTPAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	/** 데미지가 HP에 반영된 뒤 (서버) */
	FTPDamageAppliedDelegate OnDamageApplied;

protected:

	UPROPERTY(Transient)
	FTPGrantedAbilityHandles GrantedAbilityHandles;

	UPROPERTY(Transient)
	FTPGrantedAttributeSets GrantedAttributeSets;

public:

	/** 기존에 지급한 것을 회수하고 정의 에셋대로 다시 지급한다 (서버) */
	void SetUp(const UTPAbilityDefinition* AbilityDefinition, const UTPAttributeDefinition* AttributeDefinition);

	/** 정의 에셋으로 지급한 어빌리티·어트리뷰트를 모두 회수한다 (서버) */
	void Clear();

	/**
	 *  CurrentHP를 MaxHP로 채우고 사망 상태를 푼다 (서버).
	 *  지급·리스폰 같은 초기화 전용이라 값을 직접 쓴다. 전투 중 회복은 GameplayEffect로만 한다.
	 */
	void RestoreHP();

	/** HP 어트리뷰트가 데미지를 반영한 뒤 부른다. 델리게이트를 알리고 피격 이벤트를 보낸다 (죽이는 타격 포함) */
	void NotifyDamageApplied(AActor* Instigator, float AppliedDamage);

	/** 어빌리티 키 태그로 지급된 Spec을 찾는다 */
	const FGameplayAbilitySpec* FindAbilitySpecByKey(const FGameplayTag& AbilityKey) const;

	UFUNCTION(BlueprintCallable, Category="Ability")
	bool TryActivateAbilityByKey(FGameplayTag AbilityKey);

	UFUNCTION(BlueprintPure, Category="Ability")
	bool CanActivateAbilityByKey(FGameplayTag AbilityKey) const;

	UFUNCTION(BlueprintPure, Category="Ability")
	bool IsAbilityActiveByKey(FGameplayTag AbilityKey) const;

protected:

	/** ActivationPolicy가 OnSpawn인 어빌리티를 켠다 */
	void TryActivateOnSpawnAbilities();
};
