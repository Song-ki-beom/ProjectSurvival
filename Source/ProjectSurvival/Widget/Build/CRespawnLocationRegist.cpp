// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Build/CRespawnLocationRegist.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Build/CStructure_Placeable.h"
#include "Character/CSurvivor.h"
#include "ActorComponents/CInteractionComponent.h"
#include "Utility/CDebug.h"

bool UCRespawnLocationRegist::Initialize()
{
	bool Sucess = Super::Initialize();

	if (!Sucess)
		return false;

	if (!IsValid(ConfirmButton)) { CDebug::Print("ConfirmButton is invalid"); return false; }
	ConfirmButton->OnClicked.AddDynamic(this, &UCRespawnLocationRegist::OnClickConfirmButton);

	if (!IsValid(CancleButton)) { CDebug::Print("CancleButton is invalid"); return false; }
	CancleButton->OnClicked.AddDynamic(this, &UCRespawnLocationRegist::OnClickCancleButton);

	return true;
}

void UCRespawnLocationRegist::OnClickConfirmButton()
{
	if (TargetActor)
	{
		FText respawnLocationText = RespawnLocationTextBox->GetText();
		if (RespawnLocationTextBox->GetText().IsEmpty())
			return;
		SavedLocationText = respawnLocationText;
		TargetActor->SetRespawnLocationText(SavedLocationText);
		this->SetVisibility(ESlateVisibility::Collapsed);
		this->GetOwningPlayer()->SetInputMode(FInputModeGameOnly());
		this->GetOwningPlayer()->bShowMouseCursor = false;
	}
	else
		CDebug::Print("TargetActor is not Valid", FColor::Purple);
}

void UCRespawnLocationRegist::OnClickCancleButton()
{
	ACSurvivor* survivor = Cast<ACSurvivor>(this->GetOwningPlayerPawn());
	if (survivor)
	{ 
		survivor->GetInteractionComponent()->UpdateInteractionWidget();
	}
	RespawnLocationTextBox->SetText(SavedLocationText);
	this->SetVisibility(ESlateVisibility::Collapsed);
	this->GetOwningPlayer()->SetInputMode(FInputModeGameOnly());
	this->GetOwningPlayer()->bShowMouseCursor = false;
}
