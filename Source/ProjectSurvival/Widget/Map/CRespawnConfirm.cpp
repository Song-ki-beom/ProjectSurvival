// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Map/CRespawnConfirm.h"
#include "Widget/Map/CWorldMap.h"
#include "Character/CSurvivor.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "CGameInstance.h"
#include "Utility/CDebug.h"

bool UCRespawnConfirm::Initialize()
{
	bool Sucess = Super::Initialize();

	if (!Sucess)
		return false;

	if (!IsValid(ConfirmButton)) { CDebug::Print("ConfirmButton is invalid"); return false; }
	ConfirmButton->OnClicked.AddDynamic(this, &UCRespawnConfirm::OnClickConfirmButton);

	if (!IsValid(CancleButton)) { CDebug::Print("CancleButton is invalid"); return false; }
	CancleButton->OnClicked.AddDynamic(this, &UCRespawnConfirm::OnClickCancleButton);

	return true;
}

void UCRespawnConfirm::SelectRespawnLocation(FText InText, FVector InVector)
{
	RespawnLocationName->SetText(InText);
	SelectedRespawnLocation = InVector;
}

void UCRespawnConfirm::OnClickConfirmButton()
{
	ACSurvivor* survivor = Cast<ACSurvivor>(this->GetOwningPlayerPawn());
	if (survivor)
	{
		UCGameInstance* gameInstance = Cast<UCGameInstance>(GetWorld()->GetGameInstance());
		if (gameInstance)
		{
			if (survivor->HasAuthority())
			{
				survivor->BroadcastRespawnSurvivor(SelectedRespawnLocation);
				survivor->BroadcastShowPlayerLocation(gameInstance->WorldMap->GetPersonalGUID().Value);
			}
			else
			{
				survivor->RequestRespawnSurvivor(SelectedRespawnLocation);
				survivor->RequestShowPlayerLocation(gameInstance->WorldMap->GetPersonalGUID().Value);
			}

			gameInstance->WorldMap->SetVisibility(ESlateVisibility::Collapsed);
			this->SetVisibility(ESlateVisibility::Collapsed);
		}

	}
}

void UCRespawnConfirm::OnClickCancleButton()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
}
