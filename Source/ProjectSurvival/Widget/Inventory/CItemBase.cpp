// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/CItemBase.h"
#include "Utility/CDebug.h"
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
	ItemCopy->BuildData = this->BuildData;
	ItemCopy->HuntData = this->HuntData;
	
	return ItemCopy;
}


FItemData UCItemBase::CreateFItemData(UCItemBase* ItemReference)
{
	FItemData returnData;
	returnData.AssetData = ItemReference->AssetData;
	returnData.ID = ItemReference->ID;
	returnData.ItemStats = ItemReference->ItemStats;
	returnData.ItemType = ItemReference->ItemType;
	returnData.NumericData = ItemReference->NumericData;
	returnData.TextData = ItemReference->TextData;
	returnData.BuildData = ItemReference->BuildData;
	returnData.HuntData = ItemReference->HuntData;
	return returnData;
}


void UCItemBase::CopyFromItemData(FItemData ItemDataReference)
{
	this->AssetData = ItemDataReference.AssetData;
	this->ID = ItemDataReference.ID;
	this->ItemStats = ItemDataReference.ItemStats;
	this->ItemType = ItemDataReference.ItemType;
	this->NumericData = ItemDataReference.NumericData;
	this->TextData = ItemDataReference.TextData;
	this->BuildData = ItemDataReference.BuildData;
	this->HuntData = ItemDataReference.HuntData;
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


//void UCItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//	DOREPLIFETIME(UCItemBase, Quantity);
//	DOREPLIFETIME(UCItemBase, ID);
//	DOREPLIFETIME(UCItemBase, ItemType);
//	DOREPLIFETIME(UCItemBase, ItemStats);
//	DOREPLIFETIME(UCItemBase, TextData);
//	DOREPLIFETIME(UCItemBase, NumericData);
//	DOREPLIFETIME(UCItemBase, AssetData);
//	DOREPLIFETIME(UCItemBase, BuildData);
//
//
//}