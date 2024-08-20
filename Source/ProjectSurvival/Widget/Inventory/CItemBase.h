// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Struct/CItemDataStructures.h"
#include "UObject/NoExportTypes.h"
#include "Character/CSurvivor.h"
#include "CItemBase.generated.h"


UCLASS()
class PROJECTSURVIVAL_API UCItemBase : public UObject
{
	GENERATED_BODY()

public:
	/*UPROERTY()
		class UInvetoryComponent* Inventory;*/

	UCItemBase();

	UFUNCTION(Category = "Item")
	UCItemBase* CreateItemCopy();


	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemStackWeight() const
	{
		return Quantity * NumericData.Weight;
	};

	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemSingleWeight() const
	{
		return NumericData.Weight;
	};

	UFUNCTION(Category = "Item")
	FORCEINLINE bool IsFullItemStack() const 
	{
		return Quantity == NumericData.MaxStackSize;
	}

	UFUNCTION(Category = "Item")
	void SetQuantity(const int32 NewQuantity); //수량 설정

	virtual void Use(ACSurvivor* Character); //아이템 사용 

protected:
	bool operator == (const FName& OtherID) const //연사잔 오버로딩
	{
		return this->ID == OtherID;
	}


public:
	UPROPERTY(VisibleAnywhere, Category = "Item")
	int32 Quantity; //같은 아이템 항목에서 개수를 분리하는 등의 액션을 취하기 위해 따로 수량 변수 사용 (ex.) 나무 뗄감 x3 에서 한개 떼어서 버릴때)
	UPROPERTY(VisibleAnywhere, Category = "Item")
		FName ID;
	UPROPERTY(VisibleAnywhere, Category = "Item")
		EItemType ItemType;
	UPROPERTY(VisibleAnywhere, Category = "Item")
		FItemStats ItemStats;
	UPROPERTY(VisibleAnywhere, Category = "Item")
		FItemTextData TextData;
	UPROPERTY(VisibleAnywhere, Category = "Item")
		FItemNumericData NumericData;
	UPROPERTY(VisibleAnywhere, Category = "Item")
		FItemAssetData AssetData;

};
