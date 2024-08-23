// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/CItemBase.h"
#include "ActorComponents/CInventoryComponent.h"


UCItemBase::UCItemBase() : bIsCopy(false) , bIsPickup(false)
{

}

void UCItemBase::ResetItemFlags()
{
	bIsCopy = false;
	bIsPickup = false;


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
	ItemCopy->bIsCopy = true;

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

		if (Inventory) // 재고가 0개 이하면 아이템 자체를 제거 
		{
			if (Quantity <= 0)
			{
				Inventory->RemoveSingleItem(this);
			}
		}

	}

}

void UCItemBase::Use(ACSurvivor* Character)
{

}


