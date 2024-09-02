// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/CInventoryTooltip.h"
#include "Widget/Inventory/CItemBase.h"
#include "Widget/Inventory/CInventoryItemSlot.h"
#include "Components/TextBlock.h"



void UCInventoryTooltip::NativeConstruct()
{
	Super::NativeConstruct();


	UCItemBase* ItemBeingHovered = InventorySlotBeingHovered->GetItemReference();

	switch (ItemBeingHovered->ItemType)
	{
	case EItemType::Build:
		ItemType->SetText(FText::FromString("Build Item"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);

		break;

	case EItemType::Harvest:
		ItemType->SetText(FText::FromString("Harvest Tool"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		break;


	case EItemType::Hunt:
		ItemType->SetText(FText::FromString("Hunting Tool"));
		break;


	}
	ItemName->SetText(ItemBeingHovered->TextData.Name);
	DamageValue->SetText(FText::AsNumber(ItemBeingHovered->ItemStats.DamageValue));
	Usage->SetText(ItemBeingHovered->TextData.UsageText);
	ItemDescription->SetText(ItemBeingHovered->TextData.Description);
	
	const FString WeightInfo =  TEXT("무게: ") + FString::SanitizeFloat(ItemBeingHovered->GetItemStackWeight());


	StackWeight->SetText(FText::FromString(WeightInfo));


	const FString StackInfo = TEXT("최대 허용 수량: ") + FString::FromInt(ItemBeingHovered->NumericData.MaxStackSize);

	
	MaxStackSize->SetText(FText::FromString(StackInfo));
	
	



}