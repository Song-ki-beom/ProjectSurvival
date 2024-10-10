// Fill out your copyright notice in the Description page of Project Settings.


#include "CTransitionGameMode.h"
#include "Character/CSurvivor.h"
#include "Character/CSurvivorController.h"
#include "Widget/CMainHUD.h"
#include "CGameState.h"

ACTransitionGameMode::ACTransitionGameMode()
{
	////Pawn Class 세팅
	//static ConstructorHelpers::FClassFinder<APawn> defaultPawnClassFinder(TEXT("/Game/PirateIsland/Include/Blueprints/Character/Survivor/BP_CSurvivor.BP_CSurvivor_C"));
	//if (defaultPawnClassFinder.Succeeded())
	//{

	//	this->DefaultPawnClass = defaultPawnClassFinder.Class;
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("defaultPawnClassFinder failed - ACMainGameMode"));
	//}

	////PlayerController 세팅
	//static ConstructorHelpers::FClassFinder<APlayerController> playerControllerClassFinder(TEXT("/Game/PirateIsland/Include/Blueprints/Character/Survivor/BP_CSurvivorController.BP_CSurvivorController_C"));
	//if (playerControllerClassFinder.Succeeded())
	//{

	//	this->PlayerControllerClass = playerControllerClassFinder.Class;
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("playerControllerClassFinder failed - ACMainGameMode"));
	//}

	////HUD 세팅
	//static ConstructorHelpers::FClassFinder<ACMainHUD> HUDClassFinder(TEXT("/Game/PirateIsland/Include/Blueprints/Widget/BP_CMainHUD.BP_CMainHUD_C"));
	//if (HUDClassFinder.Succeeded())
	//{
	//	this->HUDClass = HUDClassFinder.Class;

	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("HUDClassFinder - ACMainGameMode"));

	//}

	////State 세팅
	//static ConstructorHelpers::FClassFinder<AActor> stateClassFinder(TEXT("/Game/PirateIsland/Include/Blueprints/BP_CGameStateBase.BP_CGameStateBase_C"));
	//if (HUDClassFinder.Succeeded())
	//{
	//	this->GameStateClass = stateClassFinder.Class;
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("stateClassFinder - ACMainGameMode"));

	//}



}

