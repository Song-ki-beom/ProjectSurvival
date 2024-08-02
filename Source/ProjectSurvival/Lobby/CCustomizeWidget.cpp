// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/CCustomizeWidget.h"
#include "Lobby/CLobbySurvivor.h"
#include "Lobby/CLobbySurvivorController.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Materials/MaterialInstanceDynamic.h"

UCCustomizeWidget::UCCustomizeWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) // ���� ������
{
	
}

bool UCCustomizeWidget::Initialize()
{
	bool Sucess = Super::Initialize();

	if (!Sucess)
		return false;

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
		UE_LOG(LogTemp, Warning, TEXT("LobbySurvivor is Valid %s"), *LobbySurvivor->GetName());

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if (!IsValid(playerController)) { UE_LOG(LogTemp, Warning, TEXT("playerController is invalid")); return; }
	else
	{
		SurvivorController = Cast<ACLobbySurvivorController>(playerController);
		playerController->SetInputMode(FInputModeUIOnly());
		playerController->bShowMouseCursor = true;
		this->bIsFocusable = true;
	}
}

void UCCustomizeWidget::TearDownWidget()
{
	this->RemoveFromViewport();
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if (!IsValid(playerController)) { UE_LOG(LogTemp, Warning, TEXT("playerController is invalid")); return; }
	else
	{
		playerController->SetInputMode(FInputModeGameAndUI());
		playerController->bShowMouseCursor = true;
		this->bIsFocusable = false;
	}
}


void UCCustomizeWidget::CreateSurvivor()
{
	TearDownWidget();
	LobbySurvivor->SetSurvivorName(SurvivorName->GetText());
	LobbySurvivor->SetLocalValue();
	ACLobbySurvivorController* lobbySurvivorController = Cast<ACLobbySurvivorController>(GetWorld()->GetFirstPlayerController());
	if (IsValid(lobbySurvivorController))
	{
		lobbySurvivorController->LoadWaitingWidget();
	}
}

void UCCustomizeWidget::ExitGame()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
}

