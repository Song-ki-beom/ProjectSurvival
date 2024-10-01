// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Map/CPlayerLocation.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Utility/CDebug.h"

void UCPlayerLocation::NativeConstruct()
{
	Super::NativeConstruct();



	//CDebug::Print("NativeConstruct Called");
	//
	//if (PlayerLocation)
	//{
	//	UTexture2D* newTexture = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/PirateIsland/Include/Textures/UI/Arrow.Arrow'"));
	//	if (newTexture)
	//	{
	//		PlayerLocation->SetBrushFromTexture(newTexture);
	//	}
	//}
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

