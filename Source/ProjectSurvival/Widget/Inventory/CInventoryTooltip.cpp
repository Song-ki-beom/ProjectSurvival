// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/CInventoryTooltip.h"
#include "Widget/Inventory/CItemBase.h"
#include "Widget/Inventory/CInventoryItemSlot.h"
#include "Components/TextBlock.h"



void UCInventoryTooltip::NativeConstruct()
{
	Super::NativeConstruct();


	UCItemBase* ItemBeingHovered = InventorySlotBeingHovered->GetItemReference();

	if (ItemBeingHovered)
	{
		switch (ItemBeingHovered->ItemType)
		{
		case EItemType::Build:
			ItemType->SetText(FText::FromString(TEXT("건축물")));
			DamageValue->SetVisibility(ESlateVisibility::Collapsed);
			break;

		case EItemType::Consumable:
			ItemType->SetText(FText::FromString(TEXT("소모품")));
			DamageValue->SetVisibility(ESlateVisibility::Collapsed);
			break;

		case EItemType::Container:
			ItemType->SetText(FText::FromString(TEXT("구조물")));
			DamageValue->SetVisibility(ESlateVisibility::Collapsed);
			break;

		case EItemType::Harvest:
			ItemType->SetText(FText::FromString(TEXT("재료")));
			DamageValue->SetVisibility(ESlateVisibility::Collapsed);
			break;

		case EItemType::Hunt:
			ItemType->SetText(FText::FromString(TEXT("장비")));
			break;
		}

		ItemName->SetText(ItemBeingHovered->TextData.Name);

		const FString WeightInfo = TEXT("무게: ") + FString::SanitizeFloat(ItemBeingHovered->GetItemStackWeight());
		StackWeight->SetText(FText::FromString(WeightInfo));

		const FString StackInfo = TEXT("슬롯당 최대 수량: ") + FString::FromInt(ItemBeingHovered->NumericData.MaxStackSize);
		MaxStackSize->SetText(FText::FromString(StackInfo));

		const FString DamageInfo = TEXT("데미지: ") + FString::SanitizeFloat(ItemBeingHovered->ItemStats.DamageValue);
		DamageValue->SetText(FText::FromString(DamageInfo));

		ItemDescription->SetText(ItemBeingHovered->TextData.Description);
	}
	
	



}