#include "Widget/Produce/CProduceDetail.h"
#include "Widget/Produce/CProduceRecipe.h"
#include "Widget/Produce/CProduceWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/RichTextBlock.h"
#include "Character/CSurvivor.h"
#include "Character/CSurvivorController.h"
#include "ActorComponents/CInventoryComponent.h"
#include "Widget/Inventory/CItemBase.h"
#include "Widget/Inventory/CInventoryPanel_Placeable.h"
#include "Build/CStructure_Placeable.h"
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

void UCProduceDetail::ProduceSurvivorItem(FName InID)
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
		for (UWidget* childWidget : ProduceDetailRecipeScroll->GetAllChildren())
		{
			if (UCProduceRecipe* recipeWidget = Cast<UCProduceRecipe>(childWidget))
			{
				int32 inventoryTotalQuantity = recipeWidget->GetInventoryQuantity();
				int32 demandQuantity = recipeWidget->GetDemandQuantity();
				recipeWidget->SetResourceQuantity(inventoryTotalQuantity - demandQuantity, demandQuantity);

				ACSurvivor* survivor = Cast<ACSurvivor>(this->GetOwningPlayerPawn());
				if (survivor)
				{
					UCInventoryComponent* inventoryComponent = survivor->GetInventoryComponent();
					if (inventoryComponent)
						CDebug::Print("InventoryComponent is valid", FColor::Magenta);

					TArray<TWeakObjectPtr<UCItemBase>> itemArray = inventoryComponent->GetInventoryContents();

					int32 usedQuantity = demandQuantity;

					// 인벤토리 루프
					for (TWeakObjectPtr<UCItemBase> itemBasePtr : itemArray)
					{
						if (UCItemBase* itemBase = itemBasePtr.Get())
						{
							CDebug::Print("itemBase is valid", FColor::Magenta);
							if (itemBase->ID == recipeWidget->GetResourceID())
							{
								if (usedQuantity == 0)
									break;

								float usedItemWeight = itemBase->NumericData.Weight;

								if (itemBase->Quantity > usedQuantity)
								{
									float inventoryTotalWeight = inventoryComponent->GetInventoryTotalWeight();
									float usedTotalWeight = usedItemWeight * usedQuantity;
									float newTotalWeight = inventoryTotalWeight - usedTotalWeight;
									inventoryComponent->SetInventoryTotalWeight(newTotalWeight);
									itemBase->SetQuantity(itemBase->Quantity - usedQuantity);
									inventoryComponent->OnInventoryUpdated.Broadcast();
									break;
								}
								else
								{
									int32 partialUsedQuantity = itemBase->Quantity;
									float inventoryTotalWeight = inventoryComponent->GetInventoryTotalWeight();
									float usedTotalWeight = usedItemWeight * partialUsedQuantity;
									float newTotalWeight = inventoryTotalWeight - usedTotalWeight;
									inventoryComponent->SetInventoryTotalWeight(newTotalWeight);
									itemBase->SetQuantity(0);
									usedQuantity -= partialUsedQuantity;
									inventoryComponent->OnInventoryUpdated.Broadcast();
								}
							}
							else
								CDebug::Print("ID not match", FColor::Magenta);
						}
						else
							CDebug::Print("itemBase is not valid", FColor::Magenta);
					}
				}
				else
					CDebug::Print("Survivor is not Valid", FColor::Magenta);
			}
		}
		UCProduceWidget* produceWidget = Cast<UCProduceWidget>(this->GetTypedOuter<UUserWidget>());
		if (produceWidget)
			produceWidget->AddProduceItemToQueue(InID);
		else
			CDebug::Print("produceWidget : is not valid", FColor::Magenta);
	}
	else
		CDebug::Print("Can't Produce");
}

void UCProduceDetail::ProducePlaceableItem(FName InID, class ACStructure_Placeable* InOwner)
{
	EPlaceableStructureType placeableStructureType = InOwner->GetPlaceableStructureType();
	if (placeableStructureType == EPlaceableStructureType::Furnace || placeableStructureType == EPlaceableStructureType::CampFire)
	{
		if (!InOwner->GetIgniteState())
			return;
	}

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
		for (UWidget* childWidget : ProduceDetailRecipeScroll->GetAllChildren())
		{
			if (UCProduceRecipe* recipeWidget = Cast<UCProduceRecipe>(childWidget))
			{
				int32 inventoryTotalQuantity = recipeWidget->GetInventoryQuantity();
				int32 demandQuantity = recipeWidget->GetDemandQuantity();
				recipeWidget->SetResourceQuantity(inventoryTotalQuantity - demandQuantity, demandQuantity);

				if (InOwner)
				{
					int32 usedQuantity = demandQuantity;

					for (UCItemBase* tempItem : InOwner->GetPlaceableInventoryWidget()->GetWidgetItems())
					{
						if (tempItem)
						{
							if (tempItem->ID == recipeWidget->GetResourceID())
							{
								if (usedQuantity == 0)
									break;

								if (tempItem->Quantity > usedQuantity)
								{
									InOwner->GetPlaceableInventoryWidget()->RemoveAmountOfItem(tempItem, usedQuantity);
									break;
								}
								else
								{
									int32 partialUsedQuantity = tempItem->Quantity;
									InOwner->GetPlaceableInventoryWidget()->RemoveItem(tempItem);
									usedQuantity -= partialUsedQuantity;
								}
							}
						}
						else
							CDebug::Print("tempItem is not Valid");
					}
				}
				else
					CDebug::Print("InOwner is not Valid", FColor::Magenta);
			}
		}

		if (this->GetOwningPlayer()->HasAuthority())
		{
			InOwner->BroadcastAddProduceItemToQueue(InID, nullptr);
		}
		else
		{
			ACSurvivorController* survivorController = Cast<ACSurvivorController>(this->GetOwningPlayer());
			if (survivorController)
				survivorController->RequestAddProduceItemToQueue(InID, InOwner);
		}
	}
	else
		CDebug::Print("Can't Produce");
}
