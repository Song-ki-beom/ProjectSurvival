#include "CGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Lobby/CLobbySurvivor.h"
#include "Lobby/CLobbyWidget.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"

#include "DrawDebugHelpers.h"
#include "Utility/CDebug.h"
#include "Net/UnrealNetwork.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Struct/CDestructibleStructures.h"
#include "Environment/CDestructibleActor.h"
#include "DestructibleComponent.h"

#include "Widget/Chatting/CChattingBox.h"
#include "Widget/Map/CWorldMap.h"
#include "Widget/Map/CMiniMap.h"
#include "Widget/World/CLoadingScreenWidget.h"

#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "MoviePlayer.h"

const static FName SESSION_NAME = TEXT("SurvivalSession");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

UCGameInstance::UCGameInstance(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> lobbyWidgetFinder(TEXT("WidgetBlueprint'/Game/PirateIsland/Include/Blueprints/Widget/Lobby/WBP_CLobbyWidget.WBP_CLobbyWidget_C'"));
	if (lobbyWidgetFinder.Succeeded())
	{
		LobbyClass = lobbyWidgetFinder.Class;
		UE_LOG(LogTemp, Warning, TEXT("Found Lobby class - UCGameInstance"));
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("userWidgetFinder Failed - UCGameInstance"));


		//Destructible Mesh DataTable Load
	static ConstructorHelpers::FObjectFinder<UDataTable> DSDataTable_BP(TEXT("DataTable'/Game/PirateIsland/Include/Datas/Widget/DT_Destructible.DT_Destructible'"));
	if (DSDataTable_BP.Succeeded())
	{
		DestructibleDataTable = DSDataTable_BP.Object;
	}

	//Hit DataTable Load
	static ConstructorHelpers::FObjectFinder<UDataTable> HitDataTable_BP(TEXT("DataTable'/Game/PirateIsland/Include/Datas/Widget/HitData/DT_HitData.DT_HitData'"));
	if (HitDataTable_BP.Succeeded())
	{
		HitDataTable = HitDataTable_BP.Object;
	}


	ConstructorHelpers::FClassFinder<UUserWidget> chattingBoxClassFinder(TEXT("WidgetBlueprint'/Game/PirateIsland/Include/Blueprints/Widget/Chatting/WBP_ChattingBox.WBP_ChattingBox_C'"));
	if (chattingBoxClassFinder.Succeeded())
		ChattingBoxClass = chattingBoxClassFinder.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> miniMapClassClassFinder(TEXT("WidgetBlueprint'/Game/PirateIsland/Include/Blueprints/Widget/Map/WBP_CMinimap.WBP_CMinimap_C'"));
	if (miniMapClassClassFinder.Succeeded())
		MiniMapClass = miniMapClassClassFinder.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> worldMapClassClassFinder(TEXT("WidgetBlueprint'/Game/PirateIsland/Include/Blueprints/Widget/Map/WBP_CWorldMap.WBP_CWorldMap_C'"));
	if (worldMapClassClassFinder.Succeeded())
		WorldMapClass = worldMapClassClassFinder.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> LoadingScreenWidgetClassFinder(TEXT("WidgetBlueprint'/Game/PirateIsland/Include/Blueprints/Widget/World/WBP_LoadingScreen.WBP_LoadingScreen_C'"));
	if (LoadingScreenWidgetClassFinder.Succeeded())
		LoadingScreenWidgetClass = LoadingScreenWidgetClassFinder.Class;

}

void UCGameInstance::Init()
{

	Super::Init();

	LoadingScreenWidget = CreateWidget<UCLoadingScreenWidget>(GetWorld(), LoadingScreenWidgetClass);
	LoadingScreenWidget->SetDesiredSizeInViewport(FVector2D(1920, 1080)); // 원하는 해상도로 설정
	LoadingScreenWidget->SetAnchorsInViewport(FAnchors(0.f, 0.f, 1.f, 1.f)); // 화면 전체를 덮도록 앵커 설정
	LoadingScreenWidget->SetAlignmentInViewport(FVector2D(0.5f, 0.5f)); // 중심에 정렬

	//잘 되는거(Single Player일때)
	//FWorldDelegates::LevelRemovedFromWorld.AddUObject(this, &UCGameInstance::OnLevelRemovedFromWorld);


	//FWorldDelegates::OnWorldPostActorTick.AddUObject(this, &UCGameInstance::OnLevelAddedToWorld);
	//FWorldDelegates::OnWorldInitializedActors.AddUObject(this, &UCGameInstance::OnLevelAddedToWorld);
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UCGameInstance::OnPreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UCGameInstance::OnPostLoadMapWithWorld);

	IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get();
	if (SubSystem != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found SubSystem %s"), *SubSystem->GetSubsystemName().ToString());
		SessionInterface = SubSystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UCGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UCGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UCGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UCGameInstance::OnJoinSessionComplete);
		}
		IdentityInterface = SubSystem->GetIdentityInterface();
		if (IdentityInterface.IsValid())
		{
			playerNetId = IdentityInterface->GetUniquePlayerId(0);
			if (playerNetId.IsValid())
			{
				FString DisplayName = IdentityInterface->GetPlayerNickname(0);
				UE_LOG(LogTemp, Warning, TEXT("Steam Display Name: %s"), *DisplayName);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SubSystem is nullptr"));
	}

	UE_LOG(LogTemp, Warning, TEXT("Found Class %s"), *LobbyClass->GetName());
}

void UCGameInstance::LoadLobbyWidget()
{
	LobbyWidget = CreateWidget<UCLobbyWidget>(this, LobbyClass);
	LobbyWidget->SetUpWidget();
	LobbyWidget->SetLobbyInterface(this);
}

void UCGameInstance::Create(FString InServerName)
{
	DesiredServerName = InServerName;
	if (SessionInterface.IsValid())
	{
		FNamedOnlineSession* ExisingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExisingSession != nullptr)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
}

void UCGameInstance::Join(uint32 InIndex)
{
	if (!SessionInterface.IsValid())
		return;
	if (!SessionSearch.IsValid())
		return;
	if (IsValid(LobbyWidget))
		LobbyWidget->TearDownWidget();
	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[InIndex]);
}

void UCGameInstance::RenewServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
			SessionSearch->bIsLanQuery = true;
		else
			SessionSearch->bIsLanQuery = false;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UCGameInstance::RemoveAllWidgets()
{
	TArray<UUserWidget*> allWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, allWidgets, UUserWidget::StaticClass(), true);

	for (UUserWidget* widget : allWidgets)
	{
		if (widget && widget->IsInViewport())
		{
			widget->RemoveFromParent();
		}
	}
}

void UCGameInstance::CreateChattingBox()
{
	if (ChattingBoxClass)
	{
		ChattingBox = CreateWidget<UCChattingBox>(this, ChattingBoxClass);

		ChattingBox->AddToViewport(6);
		ChattingBox->SetVisibility(ESlateVisibility::Visible);
		ChattingBox->bIsFocusable = true;

		FVector2D widgetSize = FVector2D(500, 200);
		ChattingBox->SetDesiredSizeInViewport(widgetSize);
		FVector2D widgetAlignment = FVector2D(-0.02, -4.35);
		ChattingBox->SetAlignmentInViewport(widgetAlignment);
		ChattingBox->SetKeyboardFocus();
	}
}

void UCGameInstance::CreateMap()
{
	if (MiniMapClass)
	{
		MiniMap = CreateWidget<UCMiniMap>(GetWorld(), MiniMapClass);
		MiniMap->AddToViewport(4);
		MiniMap->SetVisibility(ESlateVisibility::Visible);
	}

	if (WorldMapClass)
	{
		WorldMap = CreateWidget<UCWorldMap>(GetWorld(), WorldMapClass);
		WorldMap->AddToViewport(6);
		WorldMap->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCGameInstance::SetDifficultyCoeff(float InCoeff)
{
	DifficultyCoeff = InCoeff;
}

void UCGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
		return;
	}
	LobbyWidget->TearDownWidget();
	GetWorld()->ServerTravel("/Game/PirateIsland/Exclude/Maps/Waiting?listen");
	// 일정 시간 지연 후 NetDriver를 확인하는 타이머 설정
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UCGameInstance::CheckNetDriver, 1.0f, false);

}

void UCGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		CreateSession();
	}
}

void UCGameInstance::OnFindSessionComplete(bool Success)
{
	if (Success && SessionSearch.IsValid() && LobbyWidget != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found session"));
		TArray<FServerData> serverNames;
		for (const FOnlineSessionSearchResult& searchResults : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found session name %s"), *searchResults.GetSessionIdStr());
			FServerData Data;
			Data.HostUserName = searchResults.Session.OwningUserName;
			Data.MaxPlayers = searchResults.Session.SessionSettings.NumPublicConnections;
			Data.CurrentPlayers = Data.MaxPlayers - searchResults.Session.NumOpenPublicConnections;
			FString ServerName;
			if (searchResults.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
			{
				Data.Name = ServerName;
			}
			else
			{
				Data.Name = "Could not find name.";
			}
			serverNames.Add(Data);
		}
		LobbyWidget->SetServerList(serverNames);
	}
}

void UCGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid())
		return;

	FString address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, address))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect string"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"), *address);

	APlayerController* playerController = GetFirstLocalPlayerController();
	if (!playerController)
		return;
	else
		UE_LOG(LogTemp, Warning, TEXT("Found Get playerController"));

	playerController->ClientTravel(address, ETravelType::TRAVEL_Absolute);
}

void UCGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings sessionSettings;
		
		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
			sessionSettings.bIsLANMatch = true;
		else
			sessionSettings.bIsLANMatch = false;

		sessionSettings.bUsesPresence = true;
		sessionSettings.NumPublicConnections = 4;
		sessionSettings.bAllowJoinInProgress = true;
		sessionSettings.bAllowJoinViaPresence = true;
		sessionSettings.bShouldAdvertise = true;
		sessionSettings.bIsDedicated = false;
		sessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		SessionInterface->CreateSession(0, SESSION_NAME, sessionSettings);
	}
}

void UCGameInstance::CheckNetDriver()
{
	UNetDriver* NetDriver = GetWorld()->GetNetDriver();
	if (NetDriver == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("NetDriver is null"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NetDriver is valid"));
	}
}

void UCGameInstance::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	
	if (LoadingScreenWidget)
	{
		LoadingScreenWidget->AddToViewport();
	}


}

void UCGameInstance::OnLevelAddedToWorld(UWorld* World, ELevelTick TickType, float DeltaSeconds)
{
	if (LoadingScreenWidget)
	{
		//UGameViewportSubsystem::Get(InWorld)->RemoveWidget(YourWidget);
		LoadingScreenWidget->RemoveFromViewport();
	}
}

void UCGameInstance::OnPreLoadMap(const FString& MapName)
{
	if (!IsRunningDedicatedServer())
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.MoviePaths.Init("LoadingScreen", 3);
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
		//LoadingScreen.WidgetLoadingScreen = LoadingScreenWidget->TakeWidget();
		//LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
		LoadingScreen.MinimumLoadingScreenDisplayTime = 3;
		LoadingScreen.bMoviesAreSkippable = false;
		LoadingScreen.bWaitForManualStop = false;
		LoadingScreen.PlaybackType = EMoviePlaybackType::MT_LoadingLoop;
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}

	/*if (LoadingScreenWidget)
	{
		CDebug::Print(TEXT("OnPreLoadMap"));
		LoadingScreenWidget->AddToViewport();
	}*/

}

void UCGameInstance::OnPostLoadMapWithWorld(UWorld* InLoadedWorld)
{

	GetMoviePlayer()->StopMovie();

	/*if (LoadingScreenWidget)
	{
		CDebug::Print(TEXT("OnPostLoadMapWithWorld"));
		LoadingScreenWidget->RemoveFromViewport();
	}*/
}







void UCGameInstance::CreateLoadingScreen()
{
	//CDebug::Print(TEXT("Loading Screen Widget Added"));
	LoadingScreenWidget = CreateWidget<UCLoadingScreenWidget>(GetWorld(), LoadingScreenWidgetClass);
	if (LoadingScreenWidget)
	{
		LoadingScreenWidget->AddToViewport();
	}
	//BroadcastCreateLoadingScreen();
}

void UCGameInstance::BroadcastCreateLoadingScreen_Implementation()
{
	//CDebug::Print(TEXT("Loading Screen Widget Added"));
	LoadingScreenWidget = CreateWidget<UCLoadingScreenWidget>(GetWorld(), LoadingScreenWidgetClass);
	if (LoadingScreenWidget)
	{
		LoadingScreenWidget->AddToViewport();
	}

}