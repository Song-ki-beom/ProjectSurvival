#include "CLobbyGameMode.h"
#include "GameFramework/PlayerState.h"
#include "Lobby/CLobbySurvivor.h"
#include "Lobby/CWaitingWidget.h"
#include "Lobby/CLobbySurvivorController.h"

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
	++NumberOfSurvivors;
}

void ACLobbyGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}


void ACLobbyGameMode::ReadyPlayer()
{
	++NumberOfReadySurvivors;
}

bool ACLobbyGameMode::CheckPlayer()
{
	bool result = (NumberOfSurvivors == NumberOfReadySurvivors) ? true : false;
	return result;
}

void ACLobbyGameMode::StartGame()
{
	ACLobbySurvivorController* lobbySurvivorController = Cast<ACLobbySurvivorController>(GetWorld()->GetFirstPlayerController());
	bUseSeamlessTravel = true;
	GetWorld()->ServerTravel("/Game/PirateIsland/Exclude/Maps/Main?listen");
}

