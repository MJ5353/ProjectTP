#include "TPGameplayTags.h"

// ==================================================

namespace TPGameplayTags
{
	// ability key
	UE_DEFINE_GAMEPLAY_TAG(AbilityKeyTag, "Ability.Key");

	// state
	UE_DEFINE_GAMEPLAY_TAG(StateTag_Dead, "State.Dead");

	// event
	UE_DEFINE_GAMEPLAY_TAG(EventTag_Hit, "Event.Hit");

	// set by caller
	UE_DEFINE_GAMEPLAY_TAG(SetByCallerTag_Damage, "SetByCaller.Damage");
}
