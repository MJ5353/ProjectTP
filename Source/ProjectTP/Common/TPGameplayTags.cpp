#include "TPGameplayTags.h"

// ==================================================

namespace TPGameplayTags
{
	// ability key
	UE_DEFINE_GAMEPLAY_TAG(AbilityKeyTag, "Ability.Key");
	UE_DEFINE_GAMEPLAY_TAG(AbilityKeyTag_LightAttack, "Ability.Key.LightAttack");
	UE_DEFINE_GAMEPLAY_TAG(AbilityKeyTag_HeavyAttack, "Ability.Key.HeavyAttack");
	UE_DEFINE_GAMEPLAY_TAG(AbilityKeyTag_Dodge, "Ability.Key.Dodge");

	// state
	UE_DEFINE_GAMEPLAY_TAG(StateTag_Dead, "State.Dead");
	UE_DEFINE_GAMEPLAY_TAG(StateTag_Attack, "State.Attack");
	UE_DEFINE_GAMEPLAY_TAG(StateTag_ComboWindow, "State.Window.Combo");
	UE_DEFINE_GAMEPLAY_TAG(StateTag_CancelWindow, "State.Window.Cancel");

	// event
	UE_DEFINE_GAMEPLAY_TAG(EventTag_Hit, "Event.Hit");

	// set by caller
	UE_DEFINE_GAMEPLAY_TAG(SetByCallerTag_Damage, "SetByCaller.Damage");
}
