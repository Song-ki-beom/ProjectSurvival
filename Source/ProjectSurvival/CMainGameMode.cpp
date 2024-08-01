#include "CMainGameMode.h"
#include "Character/CSurvivor.h"
#include "Character/CSurvivorController.h"

ACMainGameMode::ACMainGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> defaultPawnClassFinder(TEXT("/Game/PirateIsland/Include/Blueprints/Character/Survivor/BP_CSurvivor.BP_CSurvivor_C"));
	if (defaultPawnClassFinder.Succeeded()) 
	{
		
		this->DefaultPawnClass = defaultPawnClassFinder.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("defaultPawnClassFinder failed - ACMainGameMode"));
	}

	static ConstructorHelpers::FClassFinder<APlayerController> playerControllerClassFinder(TEXT("/Game/PirateIsland/Include/Blueprints/Character/Survivor/BP_CSurvivorController.BP_CSurvivorController_C"));
	if (playerControllerClassFinder.Succeeded())
	{
		
		this->PlayerControllerClass = playerControllerClassFinder.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("playerControllerClassFinder failed - ACMainGameMode"));
	}
}
