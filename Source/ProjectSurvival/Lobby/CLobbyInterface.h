#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CLobbyInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCLobbyInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTSURVIVAL_API ICLobbyInterface
{
	GENERATED_BODY()

public:
	virtual void Create(FString InServerName) = 0;
	virtual void Join(uint32 InIndex) = 0;
	virtual void RenewServerList() = 0;
};

