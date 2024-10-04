// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Map/CPlayerLocation.h"
#include "Widget/Map/CWorldMap.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "Character/CSurvivor.h"
#include "Character/CSurvivorController.h"
#include "CGameInstance.h"
#include "Utility/CDebug.h"

void UCPlayerLocation::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCPlayerLocation::CheckWidgetOwner(bool InValue)
{
	if (InValue)
	{
		UTexture2D* redArrowTexture = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/PirateIsland/Include/Textures/UI/Arrow_Red.Arrow_Red'"));
		PlayerLocationImage->SetBrushFromTexture(redArrowTexture);
	}
	else
	{
		UTexture2D* greenArrowTexture = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/PirateIsland/Include/Textures/UI/Arrow_Green.Arrow_Green'"));
		PlayerLocationImage->SetBrushFromTexture(greenArrowTexture);
	}
}

void UCPlayerLocation::RegisterPlayer(class ACSurvivor* InActor)
{
	Survivor = InActor;
	Survivor->OnDestroyed.AddDynamic(this, &UCPlayerLocation::OnDestroyedSurvivor);
}

void UCPlayerLocation::RegisterPlayerName(const FText& InText)
{
	PlayerName->SetText(InText);
}

void UCPlayerLocation::UpdatePlayerLocation(float LocationX, float LocationY, float RotationZ)
{
	FWidgetTransform imageTransform;
	imageTransform.Translation = FVector2D(LocationX, LocationY);
	imageTransform.Angle = RotationZ;
	PlayerLocationImage->SetRenderTransform(imageTransform);

	FWidgetTransform textTransform;
	textTransform.Translation = FVector2D(LocationX, LocationY + 20.0f);
	PlayerName->SetRenderTransform(textTransform);
}

void UCPlayerLocation::RemovePlayerLocation()
{
	// 멀티캐스트로 호출되는 함수
	RemoveFromParent();
	MarkPendingKill();
}

void UCPlayerLocation::HidePlayerLocation()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UCPlayerLocation::ShowPlayerLocation()
{
	this->SetVisibility(ESlateVisibility::Visible);
}

void UCPlayerLocation::OnDestroyedSurvivor(class AActor* InActor)
{
	CDebug::Print("OnDestroyedSurvivor Called");

	FVector SpawnLocation = FVector(0.0f, 0.0f, 100.0f);
	FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);
	class ACSurvivor* newSurvivor = GetWorld()->SpawnActor<ACSurvivor>(ACSurvivor::StaticClass(), SpawnLocation, SpawnRotation);

	ACSurvivorController* survivorController = Cast<ACSurvivorController>(this->GetOwningPlayer());
	if (survivorController)
	{
		if (survivorController->HasAuthority())
		{
			survivorController->BroadcastDestroyPlayerLocation();
		}
		else
		{
			survivorController->RequestDestroyPlayerLocation();
		}
	}

	//this->GetParent()->RemoveFromParent();
	//this->GetParent()->MarkPendingKill();
}


