#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "TPAbilityDefinition.generated.h"

class UAbilitySystemComponent;
class UTPGameplayAbility;
struct FTPGrantedAbilityHandles;

/**
 *  캐릭터에 지급할 어빌리티 목록 (DataAsset)
 */
UCLASS()
class UTPAbilityDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	/** 어빌리티 키(Ability.Key.*) → 어빌리티 클래스. 키로 어빌리티를 찾고 켠다 */
	UPROPERTY(EditDefaultsOnly, Category="Ability", meta=(Categories="Ability.Key"))
	TMap<FGameplayTag, TSubclassOf<UTPGameplayAbility>> AbilityMap;

	/** 서버에서만 지급한다. 이미 지급한 키는 건너뛴다 */
	void GiveToAbilitySystem(UAbilitySystemComponent* ASC, FTPGrantedAbilityHandles& OutGrantedHandles, UObject* SourceObject = nullptr) const;
};
