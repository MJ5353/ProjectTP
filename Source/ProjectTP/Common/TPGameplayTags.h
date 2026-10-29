#pragma once

#include "NativeGameplayTags.h"

// ==================================================

namespace TPGameplayTags
{
	// ability key
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(AbilityKeyTag);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(AbilityKeyTag_LightAttack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(AbilityKeyTag_HeavyAttack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(AbilityKeyTag_Dodge);

	// state
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StateTag_Dead);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StateTag_Attack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StateTag_ComboWindow);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StateTag_CancelWindow);

	// event
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(EventTag_Hit);

	// set by caller
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCallerTag_Damage);
}
