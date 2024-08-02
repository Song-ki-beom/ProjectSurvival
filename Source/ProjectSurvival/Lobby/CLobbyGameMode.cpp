#include "CLobbyGameMode.h"
#include "GameFramework/PlayerState.h"

ACLobbyGameMode::ACLobbyGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> defaultPawnClassFinder(TEXT("/Game/PirateIsland/Include/Blueprints/Character/Survivor/BP_CLobbySurvivor.BP_CLobbySurvivor_C"));
	if (defaultPawnClassFinder.Succeeded())
	{
		this->DefaultPawnClass = defaultPawnClassFinder.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("defaultPawnClassFinder failed - ACLobbyGameMode"));
	}

	static ConstructorHelpers::FClassFinder<APlayerController> playerControllerClassFinder(TEXT("/Game/PirateIsland/Include/Blueprints/Character/Survivor/BP_CLobbySurvivorController.BP_CLobbySurvivorController_C"));
	if (playerControllerClassFinder.Succeeded())
	{

		this->PlayerControllerClass = playerControllerClassFinder.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("playerControllerClassFinder failed - ACLobbyGameMode"));
	}

	static ConstructorHelpers::FClassFinder<APlayerState> playerStateClassFinder(TEXT("/Game/PirateIsland/Include/Blueprints/Character/Survivor/BP_CLobbySurvivorState.BP_CLobbySurvivorState_C"));
	if (playerStateClassFinder.Succeeded())
	{
		this->PlayerStateClass = playerStateClassFinder.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("playerStateClassFinder failed - ACLobbyGameMode"));
	}
	
}

void ACLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++NumberOfPlayers;
}

void ACLobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--NumberOfPlayers;
}

