#include "TPGameplayTags.h"

// ==================================================

namespace TPGameplayTags
{
	// ability key
	UE_DEFINE_GAMEPLAY_TAG(AbilityKeyTag, "Ability.Key");
	UE_DEFINE_GAMEPLAY_TAG(AbilityKeyTag_LightAttack, "Ability.Key.LightAttack");
	UE_DEFINE_GAMEPLAY_TAG(AbilityKeyTag_HeavyAttack, "Ability.Key.HeavyAttack");

	// state
	UE_DEFINE_GAMEPLAY_TAG(StateTag_Dead, "State.Dead");
	UE_DEFINE_GAMEPLAY_TAG(StateTag_Attack, "State.Attack");

	// event
	UE_DEFINE_GAMEPLAY_TAG(EventTag_Hit, "Event.Hit");
	UE_DEFINE_GAMEPLAY_TAG(EventTag_ComboAvailable, "Event.Combo.Available");
	UE_DEFINE_GAMEPLAY_TAG(EventTag_ComboUnavailable, "Event.Combo.Unavailable");

	// set by caller
	UE_DEFINE_GAMEPLAY_TAG(SetByCallerTag_Damage, "SetByCaller.Damage");
}
