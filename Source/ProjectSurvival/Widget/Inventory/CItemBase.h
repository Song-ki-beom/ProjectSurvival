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

	void ResetItemFlags();


protected:
	bool operator == (const FName& OtherID) const //연산자 오버로딩
	{
		return this->ID == OtherID;
	}


public:
	UPROPERTY()
		class UCInventoryComponent* Inventory; //참고할 인벤토리

	UPROPERTY(EditAnywhere, Category = "Item")
	int32 Quantity; //같은 아이템 항목에서 개수를 분리하는 등의 액션을 취하기 위해 따로 수량 변수 사용 (ex.) 나무 뗄감 x3 에서 한개 떼어서 버릴때)
	UPROPERTY(EditAnywhere, Category = "Item")
		FName ID;
	UPROPERTY(EditAnywhere, Category = "Item")
		EItemType ItemType;
	UPROPERTY(EditAnywhere, Category = "Item")
		FItemStats ItemStats;
	UPROPERTY(EditAnywhere, Category = "Item")
		FItemTextData TextData;
	UPROPERTY(EditAnywhere, Category = "Item")
		FItemNumericData NumericData;
	UPROPERTY(EditAnywhere, Category = "Item")
		FItemAssetData AssetData;

	bool bIsCopy;
	bool bIsPickup;


};
