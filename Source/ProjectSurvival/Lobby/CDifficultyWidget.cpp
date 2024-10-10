#include "Lobby/CDifficultyWidget.h"
#include "Lobby/CLobbyGamemode.h"
#include "Lobby/CLobbySurvivor.h"
#include "Lobby/CLobbySurvivorController.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/Chatting/CChattingBox.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "CGameInstance.h"
#include "Net/UnrealNetwork.h" 
#include "CGameInstance.h"

FReply UCDifficultyWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	if (InKeyEvent.GetKey() == EKeys::Enter)
	{
		UCGameInstance* gameInstance = Cast<UCGameInstance>(GetGameInstance());
		if (gameInstance)
		{
			if (gameInstance->ChattingBox)
			{
				gameInstance->ChattingBox->SetInputMode();
			}
		}

		return FReply::Handled();
	}

	return FReply::Unhandled();
}

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

	DifficultyData = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, TEXT("DataTable'/Game/PirateIsland/Include/Datas/Widget/DT_Difficulty.DT_Difficulty'")));

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

void UCDifficultyWidget::UpdateDifficultyInfo(int InIndex)
{
	if (IsValid(DifficultyData))
	{
		switch (InIndex)
		{
		case 0:
		{
			FDifficultyInfo* difficultyInfo = DifficultyData->FindRow<FDifficultyInfo>(FName(TEXT("Easy")), TEXT("Find Easy"), true);
			DifficultyImage->SetBrushFromTexture(difficultyInfo->DifficultyTexture);
			DifficultyText->SetText(difficultyInfo->DifficultySummary);
			DifficultyDesc_1->SetText(difficultyInfo->DifficultyDetail_1);
			DifficultyDesc_2->SetText(difficultyInfo->DifficultyDetail_2);
			DifficultyDesc_3->SetText(difficultyInfo->DifficultyDetail_3);
			DifficultyDesc_4->SetText(difficultyInfo->DifficultyDetail_4);
			break;
		}
		case 1:
		{
			FDifficultyInfo* difficultyInfo = DifficultyData->FindRow<FDifficultyInfo>(FName(TEXT("Normal")), TEXT("Find Normal"), true);
			DifficultyImage->SetBrushFromTexture(difficultyInfo->DifficultyTexture);
			DifficultyText->SetText(difficultyInfo->DifficultySummary);
			DifficultyDesc_1->SetText(difficultyInfo->DifficultyDetail_1);
			DifficultyDesc_2->SetText(difficultyInfo->DifficultyDetail_2);
			DifficultyDesc_3->SetText(difficultyInfo->DifficultyDetail_3);
			DifficultyDesc_4->SetText(difficultyInfo->DifficultyDetail_4);
			break;
		}
		case 2:
		{
			FDifficultyInfo* difficultyInfo = DifficultyData->FindRow<FDifficultyInfo>(FName(TEXT("Hard")), TEXT("Find Normal"), true);
			DifficultyImage->SetBrushFromTexture(difficultyInfo->DifficultyTexture);
			DifficultyText->SetText(difficultyInfo->DifficultySummary);
			DifficultyDesc_1->SetText(difficultyInfo->DifficultyDetail_1);
			DifficultyDesc_2->SetText(difficultyInfo->DifficultyDetail_2);
			DifficultyDesc_3->SetText(difficultyInfo->DifficultyDetail_3);
			DifficultyDesc_4->SetText(difficultyInfo->DifficultyDetail_4);
			break;
		}
		case 3:
		{
			FDifficultyInfo* difficultyInfo = DifficultyData->FindRow<FDifficultyInfo>(FName(TEXT("Extreme")), TEXT("Find Normal"), true);
			DifficultyImage->SetBrushFromTexture(difficultyInfo->DifficultyTexture);
			DifficultyText->SetText(difficultyInfo->DifficultySummary);
			DifficultyDesc_1->SetText(difficultyInfo->DifficultyDetail_1);
			DifficultyDesc_2->SetText(difficultyInfo->DifficultyDetail_2);
			DifficultyDesc_3->SetText(difficultyInfo->DifficultyDetail_3);
			DifficultyDesc_4->SetText(difficultyInfo->DifficultyDetail_4);
			break;
		}
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("DifficultyData is not valid - UCDifficultyWidget"));
	}
}

void UCDifficultyWidget::UpdateClientDifficulty()
{
	switch (ServerDifficultyIndex)
	{
	case 0:
		OnEasy();
		break;
	case 1:
		OnNormal();
		break;
	case 2:
		OnHard();
		break;
	case 4:
		OnExtreme();
		break;
	}
}

void UCDifficultyWidget::OnEasy()
{
	ACLobbySurvivorController* lobbySurvivorController = Cast<ACLobbySurvivorController>(GetWorld()->GetFirstPlayerController());
	ACLobbySurvivor* lobbySurvivor = Cast<ACLobbySurvivor>(lobbySurvivorController->GetPawn());
	if (lobbySurvivor->HasAuthority())
	{
		lobbySurvivor->BroadcastSetText(0);
		ServerDifficultyIndex = 0;
	}
	UCGameInstance* gameInstance = Cast<UCGameInstance>(GetWorld()->GetGameInstance());
	gameInstance->SetDifficultyCoeff(0.75f);

	if (GetOwningPlayer()->HasAuthority())
		this->GetTypedOuter<UUserWidget>()->SetFocus();
}

void UCDifficultyWidget::OnNormal()
{
	ACLobbySurvivorController* lobbySurvivorController = Cast<ACLobbySurvivorController>(GetWorld()->GetFirstPlayerController());
	ACLobbySurvivor* lobbySurvivor = Cast<ACLobbySurvivor>(lobbySurvivorController->GetPawn());
	if (lobbySurvivor->HasAuthority())
	{
		lobbySurvivor->BroadcastSetText(1);
		ServerDifficultyIndex = 1;
	}
	UCGameInstance* gameInstance = Cast<UCGameInstance>(GetWorld()->GetGameInstance());
	gameInstance->SetDifficultyCoeff(1.0f);

	if (GetOwningPlayer()->HasAuthority())
		this->GetTypedOuter<UUserWidget>()->SetFocus();
}

void UCDifficultyWidget::OnHard()
{
	ACLobbySurvivorController* lobbySurvivorController = Cast<ACLobbySurvivorController>(GetWorld()->GetFirstPlayerController());
	ACLobbySurvivor* lobbySurvivor = Cast<ACLobbySurvivor>(lobbySurvivorController->GetPawn());
	if (lobbySurvivor->HasAuthority())
	{
		lobbySurvivor->BroadcastSetText(2);
		ServerDifficultyIndex = 2;
	}
	UCGameInstance* gameInstance = Cast<UCGameInstance>(GetWorld()->GetGameInstance());
	gameInstance->SetDifficultyCoeff(1.5f);

	if (GetOwningPlayer()->HasAuthority())
		this->GetTypedOuter<UUserWidget>()->SetFocus();
}

void UCDifficultyWidget::OnExtreme()
{
	ACLobbySurvivorController* lobbySurvivorController = Cast<ACLobbySurvivorController>(GetWorld()->GetFirstPlayerController());
	ACLobbySurvivor* lobbySurvivor = Cast<ACLobbySurvivor>(lobbySurvivorController->GetPawn());
	if (lobbySurvivor->HasAuthority())
	{
		lobbySurvivor->BroadcastSetText(3);
		ServerDifficultyIndex = 3;
	}
	UCGameInstance* gameInstance = Cast<UCGameInstance>(GetWorld()->GetGameInstance());
	gameInstance->SetDifficultyCoeff(2.5f);

	if (GetOwningPlayer()->HasAuthority())
		this->GetTypedOuter<UUserWidget>()->SetFocus();
}

void UCDifficultyWidget::OnStart()
{
	ACLobbyGameMode* lobbyGameMode = Cast<ACLobbyGameMode>(GetWorld()->GetAuthGameMode());
	ShowLoadingScreen();
	lobbyGameMode->StartGame();
}

void UCDifficultyWidget::ShowLoadingScreen()
{
	GameInstance = Cast<UCGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance)
	{
		GameInstance->CreateLoadingScreen();
	}
	
}

