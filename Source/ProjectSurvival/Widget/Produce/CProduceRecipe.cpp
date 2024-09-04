#include "Widget/Produce/CProduceRecipe.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "ActorComponents/CInventoryComponent.h"
#include "Character/CSurvivor.h"
#include "Utility/CDebug.h"

void UCProduceRecipe::SetResourceIcon(UTexture2D* InTexture2D)
{
	ResourceIcon->SetBrushFromTexture(InTexture2D);
	//bIsResourceValid = true;
}

void UCProduceRecipe::SetResourceName(FText InText)
{
	ResourceName->SetText(InText);
}

void UCProduceRecipe::SetResourceQuantity(int32 InventoryQuantity, int32 DemandQuantity)
{
	if (InventoryQuantity < DemandQuantity)
	{
		ResourceName->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
		ResourceQuantity->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
		bIsProduceable = false;
	}
	else
	{
		ResourceName->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
		ResourceQuantity->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
		bIsProduceable = true;
	}
	FText itemWeightText = FText::Format(FText::FromString(TEXT("({0}/{1})")), FText::AsNumber(InventoryQuantity), FText::AsNumber(DemandQuantity));
	ResourceQuantity->SetText(itemWeightText);
}
