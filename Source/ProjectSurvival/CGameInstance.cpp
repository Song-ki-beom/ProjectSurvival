#include "CGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Lobby/CLobbyWidget.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"

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
}

void UCGameInstance::Init()
{
	Super::Init();
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
		UE_LOG(LogTemp, Warning, TEXT("Start find session"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
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
	// ���� �ð� ���� �� NetDriver�� Ȯ���ϴ� Ÿ�̸� ����
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

		UE_LOG(LogTemp, Warning, TEXT("bIsLANMatch : %d"), sessionSettings.bIsLANMatch);
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


