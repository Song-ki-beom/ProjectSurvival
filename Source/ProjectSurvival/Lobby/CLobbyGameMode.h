#pragma once

#include "CoreMinimal.h"
#include "CMainGameMode.h"
#include "CLobbyGameMode.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACLobbyGameMode : public ACMainGameMode
{
	GENERATED_BODY()

public:
	ACLobbyGameMode();
	void PostLogin(APlayerController* NewPlayer) override;
	void Logout(AController* Exiting) override;

private:
	uint32 NumberOfPlayers = 0;
};
