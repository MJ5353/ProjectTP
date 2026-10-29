#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TPGameMode.generated.h"

// ==================================================

UCLASS(Abstract)
class PROJECTTP_API ATPGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATPGameMode();
};
