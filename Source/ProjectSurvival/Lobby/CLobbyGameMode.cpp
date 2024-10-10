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
	//// 첫 번째 플레이어 컨트롤러 가져오기
	//ACLobbySurvivorController* LobbySurvivorController = Cast<ACLobbySurvivorController>(GetWorld()->GetFirstPlayerController());

	//if (LobbySurvivorController)
	//{
	//	// 현재 포제스하고 있는 캐릭터(Pawn)를 해제
	//	APawn* CurrentPawn = LobbySurvivorController->GetPawn();
	//	if (CurrentPawn)
	//	{
	//		// 컨트롤러가 캐릭터를 소유하지 않도록 해제
	//		LobbySurvivorController->UnPossess();

	//		// 필요에 따라 캐릭터를 삭제할 수 있습니다.
	//		CurrentPawn->Destroy();  // 캐릭터 삭제
	//	}

	//	// 컨트롤러를 해제하고 삭제
	//	LobbySurvivorController->Destroy();
	//}
	bUseSeamlessTravel = true;
	GetWorld()->ServerTravel("/Game/PirateIsland/Exclude/Maps/Main?listen");
	
	/*
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACLobbyGameMode::TriggerSeamlessTravel, 0.5f, false);*/

	
}

void ACLobbyGameMode::TriggerSeamlessTravel()
{
	// Seamless Travel을 사용하도록 설정
	//bUseSeamlessTravel = true;

	/*FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	UGameplayStatics::LoadStreamLevel(this, "/Game/PirateIsland/Exclude/Maps/Main", true, true, LatentInfo);*/

	
	// 새로운 맵으로 이동
	//GetWorld()->ServerTravel("/Game/PirateIsland/Exclude/Maps/Main?listen");
}


