// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/CInventoryItemSlot.h"
#include "Widget/Inventory/CInventoryTooltip.h"
#include "Widget/Inventory/CItemBase.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"




void UCInventoryItemSlot::NativeOnInitialized() //위젯 생성될때 호출
{
	Super::NativeOnInitialized();
	if (ToolTipClass)
	{
		UCInventoryTooltip* ToolTip = CreateWidget<UCInventoryTooltip>(this, ToolTipClass);
		ToolTip->InventorySlotBeingHovered = this;
		SetToolTip(ToolTip);

	}
}

void UCInventoryItemSlot::NativeConstruct() // 위젯 생성 -> UI 그래픽 요소 결정 후 호출 
{
	Super::NativeConstruct();

	if (ItemReference)
	{
		ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);


		//아이템 중첩이 가능한 종류의 경우 
		if (ItemReference->NumericData.bIsStackable)
		{
			ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));
		}


	}

}


FReply UCInventoryItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	
	return FReply::Handled();
}

void UCInventoryItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
}

void UCInventoryItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
}

bool UCInventoryItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return false;
}
