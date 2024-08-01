#include "Lobby/CLobbyWidget.h"
#include "Lobby/CServerRow.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"

UCLobbyWidget::UCLobbyWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) // 위젯 생성자
{
	static ConstructorHelpers::FClassFinder<UUserWidget> serverRowFinder(TEXT("WidgetBlueprint'/Game/PirateIsland/Include/Blueprints/Widget/Lobby/WBP_CServerRow.WBP_CServerRow_C'"));
	if (serverRowFinder.Succeeded())
		ServerRowClass = serverRowFinder.Class;
	else
		UE_LOG(LogTemp, Warning, TEXT("serverRowFinder Failed - UCLobbyWidget"));
}

bool UCLobbyWidget::Initialize()
{
	bool Sucess = Super::Initialize();

	if (!Sucess)
		return false;

	// 버튼 바인드
	if (!IsValid(CreateButton)) { UE_LOG(LogTemp, Warning, TEXT("CreateButton is invalid")); return false; }
	CreateButton->OnClicked.AddDynamic(this, &UCLobbyWidget::SetHostServerName);

	if (!IsValid(FindButton)) { UE_LOG(LogTemp, Warning, TEXT("FindButton is invalid")); return false; }
	FindButton->OnClicked.AddDynamic(this, &UCLobbyWidget::FindServer);

	if (!IsValid(ExitButton)) { UE_LOG(LogTemp, Warning, TEXT("ExitButton is invalid")); return false; }
	ExitButton->OnClicked.AddDynamic(this, &UCLobbyWidget::ExitGame);

	if (!IsValid(JoinButton)) { UE_LOG(LogTemp, Warning, TEXT("JoinButton is invalid")); return false; }
	JoinButton->OnClicked.AddDynamic(this, &UCLobbyWidget::JoinServer);

	if (!IsValid(RenewButton)) { UE_LOG(LogTemp, Warning, TEXT("RenewButton is invalid")); return false; }
	RenewButton->OnClicked.AddDynamic(this, &UCLobbyWidget::RenewServer);

	if (!IsValid(JoinCancleButton)) { UE_LOG(LogTemp, Warning, TEXT("JoinCancleButton is invalid")); return false; }
	JoinCancleButton->OnClicked.AddDynamic(this, &UCLobbyWidget::Cancle);

	if (!IsValid(StartButton)) { UE_LOG(LogTemp, Warning, TEXT("StartButton is invalid")); return false; }
	StartButton->OnClicked.AddDynamic(this, &UCLobbyWidget::CreateServer);

	if (!IsValid(CreateCancleButton)) { UE_LOG(LogTemp, Warning, TEXT("CreateCancleButton is invalid")); return false; }
	CreateCancleButton->OnClicked.AddDynamic(this, &UCLobbyWidget::Cancle);
	
	return true;
}

void UCLobbyWidget::SetUpWidget()
{
	this->AddToViewport();
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if (!IsValid(playerController)) { UE_LOG(LogTemp, Warning, TEXT("playerController is invalid")); return; }
	else
	{
		playerController->SetInputMode(FInputModeUIOnly());
		playerController->bShowMouseCursor = true;
		this->bIsFocusable = true;
	}
}

void UCLobbyWidget::TearDownWidget()
{
	this->RemoveFromViewport();
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if (!IsValid(playerController)) { UE_LOG(LogTemp, Warning, TEXT("playerController is invalid")); return; }
	else
	{
		playerController->SetInputMode(FInputModeGameOnly());
		playerController->bShowMouseCursor = false;
		this->bIsFocusable = false;
	}
}

void UCLobbyWidget::SetLobbyInterface(ICLobbyInterface* InLobbyInterface)
{
	LobbyInterface = InLobbyInterface;
}

void UCLobbyWidget::SetServerList(TArray<FServerData> InServerName)
{
	uint32 i = 0;
	for (const FServerData& serverData : InServerName)
	{
		UCServerRow* serverRow = CreateWidget<UCServerRow>(this->GetWorld(), ServerRowClass);
		serverRow->ServerName->SetText(FText::FromString(serverData.Name));
		serverRow->HostName->SetText(FText::FromString(serverData.HostUserName));
		serverRow->Connection->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), serverData.CurrentPlayers, serverData.MaxPlayers)));
		serverRow->SetUpWidget(this, i);
		++i;
		ServerList->AddChild(serverRow);
	}
}

void UCLobbyWidget::SelectIndex(uint32 InIndex)
{
	SelectedIndex = InIndex;
	UpdateChildren();
}

void UCLobbyWidget::UpdateChildren()
{
	for (int32 i = 0; i < ServerList->GetChildrenCount(); i++)
	{
		UCServerRow* serverRow = Cast<UCServerRow>(ServerList->GetChildAt(i));
		if (serverRow != nullptr)
		{
			serverRow->bSelected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
		}
	}
}

void UCLobbyWidget::SetHostServerName()
{
	MenuSwitcher->SetActiveWidget(CreateMenu);
}

void UCLobbyWidget::CreateServer()
{
	if (LobbyInterface != nullptr)
	{
		FString ServerName = HostServerName->Text.ToString();
		LobbyInterface->Create(ServerName);
	}
}

void UCLobbyWidget::FindServer()
{
	MenuSwitcher->SetActiveWidget(JoinMenu);
	ServerList->ClearChildren();
	if (LobbyInterface != nullptr)
		LobbyInterface->RenewServerList();
}

void UCLobbyWidget::ExitGame()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
}

void UCLobbyWidget::JoinServer()
{
	if (SelectedIndex.IsSet() && LobbyInterface != nullptr)
	{
		LobbyInterface->Join(SelectedIndex.GetValue());
		UE_LOG(LogTemp, Warning, TEXT("Selected Index %d"), SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Index not set"));
	}
}

void UCLobbyWidget::RenewServer()
{
	ServerList->ClearChildren();
	if (LobbyInterface != nullptr)
		LobbyInterface->RenewServerList();
}

void UCLobbyWidget::Cancle()
{
	MenuSwitcher->SetActiveWidget(MainMenu);
}
