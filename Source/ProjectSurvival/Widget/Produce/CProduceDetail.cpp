#include "Widget/Produce/CProduceDetail.h"
#include "Widget/Produce/CProduceRecipe.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Character/CSurvivor.h"
#include "ActorComponents/CInventoryComponent.h"
#include "Widget/Inventory/CItemBase.h"
#include "Utility/CDebug.h"

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

void UCProduceDetail::AddResourceToProduceRecipeScroll(FName InID, UTexture2D* InTexture2D, FText InResourceName, int32 InventoryQuantity, int32 InDemandQuantity)
{
	UClass* widgetClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/PirateIsland/Include/Blueprints/Widget/Produce/WBP_CProduceRecipe.WBP_CProduceRecipe_C'"));
	if (widgetClass)
	{
		UCProduceRecipe* recipeWidget = CreateWidget<UCProduceRecipe>(GetWorld(), widgetClass);
		if (recipeWidget)
		{
			recipeWidget->SetResourceID(InID);
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

void UCProduceDetail::ProduceItem()
{
	int32 recipeNumber = ProduceDetailRecipeScroll->GetAllChildren().Num();
	int32 checkNumber = 0;
	for (UWidget* childWidget : ProduceDetailRecipeScroll->GetAllChildren())
	{
		if (UCProduceRecipe* recipeWidget = Cast<UCProduceRecipe>(childWidget))
		{
			if (recipeWidget->CheckProduceable())
			{
				CDebug::Print("Enough Resource");
				checkNumber++;
			}
			else
			{
				CDebug::Print("Not Enough Resource");
				break;
			}
		}
	}

	if (recipeNumber == checkNumber)
	{
		CDebug::Print("Can Produce");
		for (UWidget* childWidget : ProduceDetailRecipeScroll->GetAllChildren())
		{
			if (UCProduceRecipe* recipeWidget = Cast<UCProduceRecipe>(childWidget))
			{
				int32 inventoryQuantity = recipeWidget->GetInventoryQuantity();
				int32 demandQuantity = recipeWidget->GetDemandQuantity();
				recipeWidget->SetResourceQuantity(inventoryQuantity - demandQuantity, demandQuantity);

				ACSurvivor* survivor = Cast<ACSurvivor>(GetWorld()->GetFirstPlayerController()->GetPawn());
				if (survivor)
				{
					UCInventoryComponent* inventoryComponent = survivor->GetInventoryComponent();
					TArray<TWeakObjectPtr<UCItemBase>> itemArray = inventoryComponent->GetInventoryContents();
					
					for (TWeakObjectPtr<UCItemBase> itemBasePtr : itemArray)
					{
						if (UCItemBase* itemBase = itemBasePtr.Get())
						{
							if (itemBase->ID == recipeWidget->GetResourceID())
							{
								itemBase->SetQuantity(inventoryQuantity - demandQuantity);
								inventoryComponent->OnInventoryUpdated.Broadcast();
							}
						}
					}
				}
			}
		}
	}
	else
		CDebug::Print("Can't Produce");
}
