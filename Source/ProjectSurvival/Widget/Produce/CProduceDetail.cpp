#include "Widget/Produce/CProduceDetail.h"
#include "Widget/Produce/CProduceRecipe.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"

void UCProduceDetail::SetProduceDetailIcon(UTexture2D* InTexture2D)
{
	ProduceDetailIcon->SetBrushFromTexture(InTexture2D);
}

void UCProduceDetail::SetProduceDetailName(FText InText)
{
	ProduceDetailName->SetText(InText);
}

void UCProduceDetail::SetProduceDetailWeight(FText InText)
{
	ProduceDetailWeight->SetText(InText);
}

void UCProduceDetail::SetProduceDetailTime(FText InText)
{
	ProduceDetailTime->SetText(InText);
}

void UCProduceDetail::SetProduceDetailMethod(FText InText)
{
	ProduceDetailMethod->SetText(InText);
}

void UCProduceDetail::SetProduceDetailFlavorText(FText InText)
{
	ProduceDetailFlavorText->SetText(InText);
}

void UCProduceDetail::AddResourceToProduceRecipeScroll(UTexture2D* InTexture2D, FText InResourceName, int32 InventoryQuantity, int32 InDemandQuantity)
{
	UClass* widgetClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/PirateIsland/Include/Blueprints/Widget/Produce/WBP_CProduceRecipe.WBP_CProduceRecipe_C'"));
	if (widgetClass)
	{
		UCProduceRecipe* recipeWidget = CreateWidget<UCProduceRecipe>(GetWorld(), widgetClass);
		if (recipeWidget)
		{
			recipeWidget->SetResourceIcon(InTexture2D);
			recipeWidget->SetResourceName(InResourceName);
			recipeWidget->SetResourceQuantity(InventoryQuantity, InDemandQuantity);
			ProduceDetailRecipeScroll->AddChild(recipeWidget);
		}
	}
}

void UCProduceDetail::ClearRecipeScrollBox()
{
	ProduceDetailRecipeScroll->ClearChildren();
}
