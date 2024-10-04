#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Widget/Inventory/CItemBase.h"
#include "CGameStateBase.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
