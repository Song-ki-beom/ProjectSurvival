#include "Widget/Build/CBuildItemSlot.h"
#include "ActorComponents/CBuildComponent.h"
#include "Character/CSurvivor.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Utility/CDebug.h"

void UCBuildItemSlot::SetBuildItemSlotInfo(class UTexture2D* InTexture, FName InItemID, int32 InQuantity, TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement)
{
	StructureImage->SetBrushFromTexture(InTexture);
	StructureImage->SetVisibility(ESlateVisibility::Visible);

	FText quantityText = FText::Format(FText::FromString("x{0}"), InQuantity);
	StructureQuantityText->SetText(quantityText);
	StructureQuantityText->SetVisibility(ESlateVisibility::Visible);

	//CDebug::Print("UCBuildItemSlot InItemID : ", InItemID);
	StructureID = InItemID;
	StructureQuantity = InQuantity;
	StructureClass = InClass;
	Element = InElement;
}

void UCBuildItemSlot::ClearBuildItemSlotInfo()
{
	//CDebug::Print("ClearBuildItemSlotInfo Called");
	StructureImage->SetVisibility(ESlateVisibility::Hidden);
	StructureQuantityText->SetVisibility(ESlateVisibility::Hidden);
	StructureID = NAME_None;
	StructureClass = nullptr;
	Element = EBuildStructureElement::None;
}

void UCBuildItemSlot::SubStructureQuantity()
{
	StructureQuantity--;
	if (StructureQuantity != 0)
	{
		FText quantityText = FText::Format(FText::FromString("x{0}"), StructureQuantity);
		StructureQuantityText->SetText(quantityText);
	}
	else
	{
		ACSurvivor* survivor = Cast<ACSurvivor>(this->GetOwningPlayerPawn());
		if (survivor)
		{
			if (survivor->GetBuildComponent()->CheckIsBuilding())
			{
				survivor->CancleBuild();
			}
		}
		ClearBuildItemSlotInfo();
	}
}

void UCBuildItemSlot::SetStructureQuantity(int32 InQuantity)
{
	StructureQuantity = InQuantity;
	FText quantityText = FText::Format(FText::FromString("x{0}"), StructureQuantity);
	StructureQuantityText->SetText(quantityText);
}
