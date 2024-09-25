#include "Widget/Build/CBuildWidget.h"
#include "Widget/Build/CBuildItemSlot.h"
#include "Character/CSurvivor.h"
#include "ActorComponents/CInventoryComponent.h"
#include "Widget/Inventory/CItemBase.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Utility/CDebug.h"

void UCBuildWidget::NativeConstruct()
{
	BuildItemSlotArray.Add(BuildItemSlotQ);
	BuildItemSlotArray.Add(BuildItemSlotW);
	BuildItemSlotArray.Add(BuildItemSlotE);
	BuildItemSlotArray.Add(BuildItemSlotA);
	BuildItemSlotArray.Add(BuildItemSlotS);
	BuildItemSlotArray.Add(BuildItemSlotD);
	BuildItemSlotArray.Add(BuildItemSlotZ);
	BuildItemSlotArray.Add(BuildItemSlotX);
	BuildItemSlotArray.Add(BuildItemSlotC);

	Survivor = Cast<ACSurvivor>(this->GetOwningPlayerPawn());
}

void UCBuildWidget::SaveStructureInfo(ESelectedStructure InKey, UTexture2D* InTexture, TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement)
{
	switch (InKey)
	{
	case ESelectedStructure::Q:
	{
		//TEST
		FName tempID = "Build_1";
		//TEST

		RemoveSameBuildItemSlot(tempID);
		int32 slotQuantity = GetSameItemQuantity(tempID);
		BuildItemSlotQ->SetBuildItemSlotInfo(InTexture, tempID, slotQuantity, InClass, InElement);
		break;
	}
	case ESelectedStructure::W:
	{
		//TEST
		FName tempID = "Build_16";
		//TEST

		RemoveSameBuildItemSlot(tempID);
		int32 slotQuantity = GetSameItemQuantity(tempID);
		CDebug::Print("Saved tempID = ", tempID);
		BuildItemSlotW->SetBuildItemSlotInfo(InTexture, tempID, slotQuantity, InClass, InElement);
		break;
	}
	default:
		break;
	}
	//switch (InKey)
	//{
	//case ESelectedStructure::Q:
	//{
	//	// Test
	//	//StructureInfo_Q.StructureID = "Build_1";
	//	FText structureQuantityText = FText::Format(FText::FromString("x{0}"), 3);
	//	StructureQuantityQ->SetText(structureQuantityText);
	//	// Test

	//	StructureImageQ->SetVisibility(ESlateVisibility::Visible);
	//	StructureImageQ->SetBrushFromTexture(InTexture);
	//	StructureInfo_Q.StructureClass = InClass;
	//	StructureInfo_Q.Element = InElement;
	//	break;
	//}
	//case ESelectedStructure::W:
	//{
	//	StructureImageW->SetVisibility(ESlateVisibility::Visible);
	//	StructureImageW->SetBrushFromTexture(InTexture);
	//	StructureInfo_W.StructureClass = InClass;
	//	StructureInfo_W.Element = InElement;
	//	break;
	//}
	//case ESelectedStructure::E:
	//{
	//	StructureImageE->SetVisibility(ESlateVisibility::Visible);
	//	StructureImageE->SetBrushFromTexture(InTexture);
	//	StructureInfo_E.StructureClass = InClass;
	//	StructureInfo_E.Element = InElement;
	//	break;
	//}
	//case ESelectedStructure::A:
	//{
	//	StructureImageA->SetVisibility(ESlateVisibility::Visible);
	//	StructureImageA->SetBrushFromTexture(InTexture);
	//	StructureInfo_A.StructureClass = InClass;
	//	StructureInfo_A.Element = InElement;
	//	break;
	//}
	//case ESelectedStructure::S:
	//{
	//	StructureImageS->SetVisibility(ESlateVisibility::Visible);
	//	StructureImageS->SetBrushFromTexture(InTexture);
	//	StructureInfo_S.StructureClass = InClass;
	//	StructureInfo_S.Element = InElement;
	//	break;
	//}
	//case ESelectedStructure::D:
	//{
	//	StructureImageD->SetVisibility(ESlateVisibility::Visible);
	//	StructureImageD->SetBrushFromTexture(InTexture);
	//	StructureInfo_D.StructureClass = InClass;
	//	StructureInfo_D.Element = InElement;
	//	break;
	//}
	//case ESelectedStructure::Z:
	//{
	//	StructureImageZ->SetVisibility(ESlateVisibility::Visible);
	//	StructureImageZ->SetBrushFromTexture(InTexture);
	//	StructureInfo_Z.StructureClass = InClass;
	//	StructureInfo_Z.Element = InElement;
	//	break;
	//}
	//case ESelectedStructure::X:
	//{
	//	StructureImageX->SetVisibility(ESlateVisibility::Visible);
	//	StructureImageX->SetBrushFromTexture(InTexture);
	//	StructureInfo_X.StructureClass = InClass;
	//	StructureInfo_X.Element = InElement;
	//	break;
	//}
	//case ESelectedStructure::C:
	//{
	//	StructureImageC->SetVisibility(ESlateVisibility::Visible);
	//	StructureImageC->SetBrushFromTexture(InTexture);
	//	StructureInfo_C.StructureClass = InClass;
	//	StructureInfo_C.Element = InElement;
	//	break;
	//}
	//}
}

TSubclassOf<ACStructure> UCBuildWidget::GetStructureClass(ESelectedStructure InKey)
{
	switch (InKey)
	{
	case ESelectedStructure::Q:
	{
		if (IsValid(BuildItemSlotQ->GetBuildItemSlotClass()))
			return BuildItemSlotQ->GetBuildItemSlotClass();
		break;
	}
	case ESelectedStructure::W:
	{
		if (IsValid(BuildItemSlotW->GetBuildItemSlotClass()))
			return BuildItemSlotW->GetBuildItemSlotClass();
		break;
	}
	case ESelectedStructure::E:
	{
		if (IsValid(BuildItemSlotE->GetBuildItemSlotClass()))
			return BuildItemSlotE->GetBuildItemSlotClass();
		break;
	}
	case ESelectedStructure::A:
	{
		if (IsValid(BuildItemSlotA->GetBuildItemSlotClass()))
			return BuildItemSlotA->GetBuildItemSlotClass();
		break;
	}
	case ESelectedStructure::S:
	{
		if (IsValid(BuildItemSlotS->GetBuildItemSlotClass()))
			return BuildItemSlotS->GetBuildItemSlotClass();
		break;
	}
	case ESelectedStructure::D:
	{
		if (IsValid(BuildItemSlotD->GetBuildItemSlotClass()))
			return BuildItemSlotD->GetBuildItemSlotClass();
		break;
	}
	case ESelectedStructure::Z:
	{
		if (IsValid(BuildItemSlotZ->GetBuildItemSlotClass()))
			return BuildItemSlotZ->GetBuildItemSlotClass();
		break;
	}
	case ESelectedStructure::X:
	{
		if (IsValid(BuildItemSlotX->GetBuildItemSlotClass()))
			return BuildItemSlotX->GetBuildItemSlotClass();
		break;
	}
	case ESelectedStructure::C:
	{
		if (IsValid(BuildItemSlotC->GetBuildItemSlotClass()))
			return BuildItemSlotC->GetBuildItemSlotClass();
		break;
	}
	}
	return nullptr;
}

EBuildStructureElement UCBuildWidget::GetStructureElement(ESelectedStructure InKey)
{
	switch (InKey)
	{
	case ESelectedStructure::Q:
	{
		if (IsValid(BuildItemSlotQ->GetBuildItemSlotClass()))
			return BuildItemSlotQ->GetBuildItemSlotElement();
		break;
	}
	case ESelectedStructure::W:
	{
		if (IsValid(BuildItemSlotW->GetBuildItemSlotClass()))
			return BuildItemSlotW->GetBuildItemSlotElement();
		break;
	}
	case ESelectedStructure::E:
	{
		if (IsValid(BuildItemSlotE->GetBuildItemSlotClass()))
			return BuildItemSlotE->GetBuildItemSlotElement();
		break;
	}
	case ESelectedStructure::A:
	{
		if (IsValid(BuildItemSlotA->GetBuildItemSlotClass()))
			return BuildItemSlotA->GetBuildItemSlotElement();
		break;
	}
	case ESelectedStructure::S:
	{
		if (IsValid(BuildItemSlotS->GetBuildItemSlotClass()))
			return BuildItemSlotS->GetBuildItemSlotElement();
		break;
	}
	case ESelectedStructure::D:
	{
		if (IsValid(BuildItemSlotD->GetBuildItemSlotClass()))
			return BuildItemSlotD->GetBuildItemSlotElement();
		break;
	}
	case ESelectedStructure::Z:
	{
		if (IsValid(BuildItemSlotZ->GetBuildItemSlotClass()))
			return BuildItemSlotZ->GetBuildItemSlotElement();
		break;
	}
	case ESelectedStructure::X:
	{
		if (IsValid(BuildItemSlotX->GetBuildItemSlotClass()))
			return BuildItemSlotX->GetBuildItemSlotElement();
		break;
	}
	case ESelectedStructure::C:
	{
		if (IsValid(BuildItemSlotC->GetBuildItemSlotClass()))
			return BuildItemSlotC->GetBuildItemSlotElement();
		break;
	}
	}
	return EBuildStructureElement::None;
}

void UCBuildWidget::RefreshBuildWidget()
{
	for (UCBuildItemSlot* itemSlot : BuildItemSlotArray)
	{
		if (itemSlot->GetBuildItemSlotID().IsNone())
			continue;
		
		if (itemSlot->GetBuildItemSlotQuantity() == 0)
		{
			itemSlot->ClearBuildItemSlotInfo();
		}
	}
}

void UCBuildWidget::RemoveSameBuildItemSlot(FName InItemID)
{
	for (UCBuildItemSlot* itemSlot : BuildItemSlotArray)
	{
		if (itemSlot->GetBuildItemSlotID() == InItemID)
		{
			itemSlot->ClearBuildItemSlotInfo();
			break;
		}
	}
}

int32 UCBuildWidget::GetSameItemQuantity(FName InItemID)
{
	if (Survivor)
	{
		int32 quantity = 0;
		for (TWeakObjectPtr<UCItemBase> itemPtr : Survivor->GetInventoryComponent()->GetInventoryContents())
		{
			if (itemPtr->ID == InItemID)
				quantity += itemPtr->Quantity;
		}
		return quantity;
	}
	return 0;
}
//
//void UCBuildWidget::RefreshBuildWidget(FName InItemID)
//{
//	for (FStructureInfo structureInfo : StructureInfo_Array)
//	{
//		if (structureInfo.StructureID.IsNone() || structureInfo.StructureID != InItemID)
//			continue;
//
//		if (Survivor)
//		{
//			int32 quantity = 0;
//			for (TWeakObjectPtr<UCItemBase> itemPtr : Survivor->GetInventoryComponent()->GetInventoryContents())
//			{
//				if (itemPtr->ID == InItemID)
//					quantity += itemPtr->Quantity;
//			}
//			structureInfo.StructureQuantity = quantity;
//		}
//	}
//}
