#pragma once

#include "NativeGameplayTags.h"

/**
 *  프로젝트의 네이티브 GameplayTag를 한 곳에 모아 둔다.
 *  새 태그는 여기와 TPGameplayTags.cpp에 같이 추가한다.
 */
namespace TPGameplayTags
{
	// 어빌리티 키. 지급할 때 Spec의 동적 태그로 붙여서 어빌리티를 찾는 데 쓴다
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(AbilityKey);

	// 유닛 상태
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Dead);

	// 게임플레이 이벤트
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Hit);

	// SetByCaller
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Damage);
}
