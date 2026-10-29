#pragma once

#include "NativeGameplayTags.h"

// ==================================================

namespace TPGameplayTags
{
	// ability key
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(AbilityKeyTag);

	// state
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StateTag_Dead);

	// event
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(EventTag_Hit);

	// set by caller
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCallerTag_Damage);
}
