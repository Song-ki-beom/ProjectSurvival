// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/CCustomizeWidget.h"
#include "Lobby/CLobbySurvivor.h"
#include "Lobby/CLobbySurvivorController.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Engine/TextureRenderTarget2D.h"
#include "CGameInstance.h"
#include "Utility/CDebug.h"

UCCustomizeWidget::UCCustomizeWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) // 위젯 생성자
{
	
}

bool UCCustomizeWidget::Initialize()
{
	bool Sucess = Super::Initialize();

	if (!Sucess)
		return false;

	if (!IsValid(HairLeftButton)) { UE_LOG(LogTemp, Warning, TEXT("HairLeftButton is invalid")); return false; }
	HairLeftButton->OnClicked.AddDynamic(this, &UCCustomizeWidget::SubHairIndex);
	if (!IsValid(HairRightButton)) { UE_LOG(LogTemp, Warning, TEXT("HairRightButton is invalid")); return false; }
	HairRightButton->OnClicked.AddDynamic(this, &UCCustomizeWidget::AddHairIndex);

	if (!IsValid(HairColorLeftButton)) { UE_LOG(LogTemp, Warning, TEXT("HairColorLeftButton is invalid")); return false; }
	HairColorLeftButton->OnClicked.AddDynamic(this, &UCCustomizeWidget::SubHairColorIndex);
	if (!IsValid(HairColorRightButton)) { UE_LOG(LogTemp, Warning, TEXT("HairColorRightButton is invalid")); return false; }
	HairColorRightButton->OnClicked.AddDynamic(this, &UCCustomizeWidget::AddHairColorIndex);

	if (!IsValid(PantsLeftButton)) { UE_LOG(LogTemp, Warning, TEXT("PantsLeftButton is invalid")); return false; }
	PantsLeftButton->OnClicked.AddDynamic(this, &UCCustomizeWidget::SubPantsIndex);
	if (!IsValid(PantsRightButton)) { UE_LOG(LogTemp, Warning, TEXT("PantsRightButton is invalid")); return false; }
	PantsRightButton->OnClicked.AddDynamic(this, &UCCustomizeWidget::AddPantsIndex);

	if (!IsValid(BootsLeftButton)) { UE_LOG(LogTemp, Warning, TEXT("BootsLeftButton is invalid")); return false; }
	BootsLeftButton->OnClicked.AddDynamic(this, &UCCustomizeWidget::SubBootsIndex);
	if (!IsValid(BootsRightButton)) { UE_LOG(LogTemp, Warning, TEXT("BootsRightButton is invalid")); return false; }
	BootsRightButton->OnClicked.AddDynamic(this, &UCCustomizeWidget::AddBootsIndex);

	if (!IsValid(SkinColorLeftButton)) { UE_LOG(LogTemp, Warning, TEXT("SkinColorLeftButton is invalid")); return false; }
	SkinColorLeftButton->OnClicked.AddDynamic(this, &UCCustomizeWidget::SubSkinColorIndex);
	if (!IsValid(SkinColorRightButton)) { UE_LOG(LogTemp, Warning, TEXT("SkinColorRightButton is invalid")); return false; }
	SkinColorRightButton->OnClicked.AddDynamic(this, &UCCustomizeWidget::AddSkinColorIndex);

	if (!IsValid(CreateButton)) { UE_LOG(LogTemp, Warning, TEXT("CreateButton is invalid")); return false; }
	CreateButton->OnClicked.AddDynamic(this, &UCCustomizeWidget::CreateSurvivor);

	if (!IsValid(ExitButton)) { UE_LOG(LogTemp, Warning, TEXT("ExitButton is invalid")); return false; }
	ExitButton->OnClicked.AddDynamic(this, &UCCustomizeWidget::ExitGame);

	return true;
}

void UCCustomizeWidget::SetUpWidget()
{
	this->AddToViewport();

	LobbySurvivor = Cast<ACLobbySurvivor>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (IsValid(LobbySurvivor))
	{
		UE_LOG(LogTemp, Warning, TEXT("LobbySurvivor is Valid %s"), *LobbySurvivor->GetName());
	}
	else
	{
		CDebug::Print("Lobby Survivor is not valid");
	}

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if (!IsValid(playerController)) { UE_LOG(LogTemp, Warning, TEXT("playerController is invalid")); return; }
	else
	{
		SurvivorController = Cast<ACLobbySurvivorController>(playerController);
		playerController->SetInputMode(FInputModeUIOnly());
		playerController->bShowMouseCursor = true;
		this->bIsFocusable = true;
	}

	ACLobbySurvivorController* lobbySurvivorController = Cast<ACLobbySurvivorController>(GetWorld()->GetFirstPlayerController());
	if (IsValid(lobbySurvivorController))
	{
		lobbySurvivorController->LoadWaitingWidget();
	}
}

void UCCustomizeWidget::TearDownWidget()
{
	this->RemoveFromViewport();
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if (!IsValid(playerController)) { UE_LOG(LogTemp, Warning, TEXT("playerController is invalid")); return; }
	else
	{
		if (playerController->GetPawn()->HasAuthority())
		{
			playerController->SetInputMode(FInputModeGameAndUI());
			playerController->bShowMouseCursor = true;
			this->bIsFocusable = false;
		}
		else
		{
			playerController->SetInputMode(FInputModeGameOnly());
			playerController->bShowMouseCursor = false;
			this->bIsFocusable = true;
		}
	}
}

void UCCustomizeWidget::GetCustomizeInfo()
{
	HairIndex = 0;
	MaxHairIndex = LobbySurvivor->GetHairRowNumber();
	HairColorIndex = 0;
	MaxHairColorIndex = LobbySurvivor->GetHairColorRowNumber();
	PantsIndex = 0;
	MaxPantsIndex = LobbySurvivor->GetPantsRowNumber();
	BootsIndex = 0;
	MaxBootsIndex = LobbySurvivor->GetBootsRowNumber();
	SkinColorIndex = 0;
	MaxSkinColorIndex = LobbySurvivor->GetSkinColorRowNumber();
}

void UCCustomizeWidget::SubHairIndex()
{
	if (HairIndex > 0)
	{
		--HairIndex;
		SetHairIndexText();
		CDebug::Print("HairIndex--");
	}
}

void UCCustomizeWidget::AddHairIndex()
{
	if (HairIndex < MaxHairIndex - 1)
	{
		++HairIndex;
		SetHairIndexText();
		CDebug::Print("HairIndex++");
	}
}

void UCCustomizeWidget::SetHairIndexText()
{
	CDebug::Print("SetHair Called");
	HairIndexText->SetText(FText::AsNumber(HairIndex + 1));
	LobbySurvivor->SetHeadMesh(HairIndex + 1);
}

void UCCustomizeWidget::SubHairColorIndex()
{
	if (HairColorIndex > 0)
	{
		--HairColorIndex;
		SetHairColorIndexText();
	}
}

void UCCustomizeWidget::AddHairColorIndex()
{
	if (HairColorIndex < MaxHairColorIndex - 1)
	{
		++HairColorIndex;
		SetHairColorIndexText();
	}
}

void UCCustomizeWidget::SetHairColorIndexText()
{
	HairColorIndexText->SetText(FText::AsNumber(HairColorIndex + 1));
	LobbySurvivor->SetHeadMeshColor(HairColorIndex + 1);
}

void UCCustomizeWidget::SubPantsIndex()
{
	if (PantsIndex > 0)
	{
		--PantsIndex;
		SetPantsIndexText();
	}
}

void UCCustomizeWidget::AddPantsIndex()
{
	if (PantsIndex < MaxPantsIndex - 1)
	{
		++PantsIndex;
		SetPantsIndexText();
	}
}

void UCCustomizeWidget::SetPantsIndexText()
{
	PantsIndexText->SetText(FText::AsNumber(PantsIndex + 1));
	LobbySurvivor->SetPantsMesh(PantsIndex + 1);
}

void UCCustomizeWidget::SubBootsIndex()
{
	if (BootsIndex > 0)
	{
		--BootsIndex;
		SetBootsIndexText();
	}
}

void UCCustomizeWidget::AddBootsIndex()
{
	if (BootsIndex < MaxBootsIndex - 1)
	{
		++BootsIndex;
		SetBootsIndexText();
	}
}

void UCCustomizeWidget::SetBootsIndexText()
{
	BootsIndexText->SetText(FText::AsNumber(BootsIndex + 1));
	LobbySurvivor->SetBootsMesh(BootsIndex + 1);
}

void UCCustomizeWidget::SubSkinColorIndex()
{
	if (SkinColorIndex > 0)
	{
		--SkinColorIndex;
		SetSkinColorIndexText();
	}
}

void UCCustomizeWidget::AddSkinColorIndex()
{
	if (SkinColorIndex < MaxSkinColorIndex - 1)
	{
		++SkinColorIndex;
		SetSkinColorIndexText();
	}
}

void UCCustomizeWidget::SetSkinColorIndexText()
{
	SkinColorIndexText->SetText(FText::AsNumber(SkinColorIndex + 1));
	LobbySurvivor->SetSkinColor(SkinColorIndex + 1);
}

void UCCustomizeWidget::CreateSurvivor()
{
	TearDownWidget();
	LobbySurvivor->SetLocalValue();
	LobbySurvivor->SetSurvivorName(SurvivorName->GetText());
	LobbySurvivor->RequestReady();
	ACLobbySurvivorController* lobbySurvivorController = Cast<ACLobbySurvivorController>(GetWorld()->GetFirstPlayerController());
	if (IsValid(lobbySurvivorController))
	{
		lobbySurvivorController->SetVisibleWaitingWidget();
	}
	UCGameInstance* gameInstance = Cast<UCGameInstance>(GetWorld()->GetGameInstance());
	gameInstance->SaveSurvivorName(SurvivorName->GetText());
	gameInstance->SaveCustomizeRowNames(LobbySurvivor->GetCustomizeRowNames());
}

void UCCustomizeWidget::ExitGame()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
}

