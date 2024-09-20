// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/CEarnInfoSlot.h"
#include "Widget/Inventory/CItemBase.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Utility/CDebug.h"

void UCEarnInfoSlot::NativeConstruct()
{
	Super::NativeConstruct();

}

void UCEarnInfoSlot::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	const UCItemBase* ItemReference = Cast<UCItemBase>(ListItemObject);
	if (ItemReference == nullptr)
		return;
	else
		CDebug::Print("ItemReference is not Valid");
		
	CDebug::Print("Earn");

	ItemName->SetText(FText(ItemReference->TextData.Name));
	ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);
	QuantityText->SetText(FText::AsNumber(ItemReference->Quantity));

}