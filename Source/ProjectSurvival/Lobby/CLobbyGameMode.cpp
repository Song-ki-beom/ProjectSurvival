#include "CLobbyGameMode.h"
#include "GameFramework/PlayerState.h"
#include "Lobby/CLobbySurvivor.h"
#include "Lobby/CWaitingWidget.h"
#include "Lobby/CLobbySurvivorController.h"
#include "Net/UnrealNetwork.h" 
#include "Kismet/GameplayStatics.h"
#include "Utility/CDebug.h"

ACLobbyGameMode::ACLobbyGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	
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
}

void ACLobbyGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ACLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++NumberOfSurvivors;
}

void ACLobbyGameMode::ReadyPlayer()
{
	++NumberOfReadySurvivors;
}

void ACLobbyGameMode::StartGame()
{
	bUseSeamlessTravel = true;
	GetWorld()->ServerTravel("/Game/PirateIsland/Exclude/Maps/Main?listen");
}

bool ACLobbyGameMode::CheckPlayer()
{
	bool result = (NumberOfSurvivors == NumberOfReadySurvivors) ? true : false;
	return result;
}