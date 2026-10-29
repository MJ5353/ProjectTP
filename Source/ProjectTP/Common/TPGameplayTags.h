#pragma once

#include "NativeGameplayTags.h"

// ==================================================

namespace TPGameplayTags
{
	// ability key
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(AbilityKeyTag);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(AbilityKeyTag_LightAttack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(AbilityKeyTag_HeavyAttack);

	// state
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StateTag_Dead);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StateTag_Attack);

	// event
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(EventTag_Hit);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(EventTag_ComboAvailable);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(EventTag_ComboUnavailable);

	// set by caller
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCallerTag_Damage);
}
