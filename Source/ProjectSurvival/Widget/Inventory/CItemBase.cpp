// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/CItemBase.h"


UCItemBase::UCItemBase()
{

}

UCItemBase* UCItemBase::CreateItemCopy()
{
	UCItemBase* ItemCopy = NewObject<UCItemBase>(StaticClass());
	ItemCopy->ID = this->ID;
	ItemCopy->Quantity = this->Quantity;
	ItemCopy->ItemType = this->ItemType;
	ItemCopy->TextData = this->TextData;
	ItemCopy->ItemStats = this->ItemStats;
	ItemCopy->NumericData = this->NumericData;
	ItemCopy->AssetData = this->AssetData;

	return ItemCopy;
}

void UCItemBase::SetQuantity(const int32 NewQuantity)
{
	if (NewQuantity != Quantity)
	{
		int32 size=1; //최소 개수
		if (NumericData.bIsStackable)
		{
			size = NumericData.MaxStackSize;
		}
		Quantity = FMath::Clamp(NewQuantity, 0, size);

		/*if (Inventory)
		{
			if (Quantity <= 0)
			{
				Inventory->RemoveItem(this);
			}
		}*/

	}

}

void UCItemBase::Use(ACSurvivor* Character)
{

}
