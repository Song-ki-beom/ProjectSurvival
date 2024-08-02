#include "Lobby/CDifficultyWidget.h"
#include "Lobby/CLobbyGamemode.h"
#include "Lobby/CLobbySurvivor.h"
#include "Lobby/CLobbySurvivorController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

bool UCDifficultyWidget::Initialize()
{
	bool Sucess = Super::Initialize();

	if (!Sucess)
		return false;

	if (!IsValid(EasyButton)) { UE_LOG(LogTemp, Warning, TEXT("EasyButton is invalid")); return false; }
	EasyButton->OnClicked.AddDynamic(this, &UCDifficultyWidget::OnEasy);

	if (!IsValid(NormalButton)) { UE_LOG(LogTemp, Warning, TEXT("NormalButton is invalid")); return false; }
	NormalButton->OnClicked.AddDynamic(this, &UCDifficultyWidget::OnNormal);

	if (!IsValid(HardButton)) { UE_LOG(LogTemp, Warning, TEXT("HardButton is invalid")); return false; }
	HardButton->OnClicked.AddDynamic(this, &UCDifficultyWidget::OnHard);

	if (!IsValid(ExtremeButton)) { UE_LOG(LogTemp, Warning, TEXT("ExtremeButton is invalid")); return false; }
	ExtremeButton->OnClicked.AddDynamic(this, &UCDifficultyWidget::OnExtreme);

	if (!IsValid(StartButton)) { UE_LOG(LogTemp, Warning, TEXT("StartButton is invalid")); return false; }
	StartButton->OnClicked.AddDynamic(this, &UCDifficultyWidget::OnStart);

	return true;
}

void UCDifficultyWidget::ActivateStartButton()
{
	StartSwitcher->SetActiveWidgetIndex(0);
}

void UCDifficultyWidget::DeactivateStartButton()
{
	StartSwitcher->SetActiveWidgetIndex(1);
}

void UCDifficultyWidget::SetClientStartButton()
{
	StartSwitcher->SetActiveWidgetIndex(2);
}

void UCDifficultyWidget::OnEasy()
{
	DifficultyDesc_1->SetText(FText::FromString(TEXT("이지")));
	ACLobbySurvivorController* lobbySurvivorController = Cast<ACLobbySurvivorController>(GetWorld()->GetFirstPlayerController());
	ACLobbySurvivor* lobbySurvivor = Cast<ACLobbySurvivor>(lobbySurvivorController->GetPawn());
	if (lobbySurvivor->HasAuthority())
		lobbySurvivor->BroadcastSetText();
}

void UCDifficultyWidget::OnNormal()
{
	DifficultyDesc_1->SetText(FText::FromString(TEXT("노말")));
}

void UCDifficultyWidget::OnHard()
{
	DifficultyDesc_1->SetText(FText::FromString(TEXT("하드")));
}

void UCDifficultyWidget::OnExtreme()
{
	DifficultyDesc_1->SetText(FText::FromString(TEXT("익스")));
}

void UCDifficultyWidget::OnStart()
{
	ACLobbyGameMode* lobbyGameMode = Cast<ACLobbyGameMode>(GetWorld()->GetAuthGameMode());
	lobbyGameMode->StartGame();
}



