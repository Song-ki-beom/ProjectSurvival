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
		return ID == OtherID;
	}


public:
	UPROPERTY(VisibleAnywhere, Category = "Item Data" , meta =(UIMin=1 ,UIMax = 100))
	int32 Quantity; //같은 아이템 항목에서 개수를 분리하는 등의 액션을 취하기 위해 따로 수량 변수 사용 (ex.) 나무 뗄감 x3 에서 한개 떼어서 버릴때)
	UPROPERTY(EditAnywhere, Category = "Item Data")
		FName ID;
	UPROPERTY(EditAnywhere, Category = "Item Data")
		EItemType ItemType;
	UPROPERTY(EditAnywhere, Category = "Item Data")
		FItemStats ItemStats;
	UPROPERTY(EditAnywhere, Category = "Item Data")
		FItemTextData TextData;
	UPROPERTY(EditAnywhere, Category = "Item Data")
		FItemNumericData NumericData;
	UPROPERTY(EditAnywhere, Category = "Item Data")
		FItemAssetData AssetData;

};
