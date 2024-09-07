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
	
	//Drop & PickUp Item Spawn  리플리케이트를 위한 Struct 복제 전달용 
	UFUNCTION(Category = "Item")
	FItemData CreateFItemData(class UCItemBase* ItemReference);
	UFUNCTION(Category = "Item")
	void CopyFromItemData(FItemData ItemDataReference);

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
	UPROPERTY()
		class UCActorInventoryComponent* ActorInventory; //참고할 액터인벤토리

	UPROPERTY(EditAnywhere, Category = "Item" , Replicated)
	int32 Quantity; //같은 아이템 항목에서 개수를 분리하는 등의 액션을 취하기 위해 따로 수량 변수 사용 (ex.) 나무 뗄감 x3 에서 한개 떼어서 버릴때)
	UPROPERTY(EditAnywhere, Category = "Item", Replicated)
		FName ID;
	UPROPERTY(EditAnywhere, Category = "Item", Replicated)
		EItemType ItemType;
	UPROPERTY(EditAnywhere, Category = "Item", Replicated)
		FItemStats ItemStats;
	UPROPERTY(EditAnywhere, Category = "Item", Replicated)
		FItemTextData TextData;
	UPROPERTY(EditAnywhere, Category = "Item", Replicated)
		FItemNumericData NumericData;
	UPROPERTY(EditAnywhere, Category = "Item", Replicated)
		FItemAssetData AssetData;

	bool bIsCopy;
	bool bIsPickup;


};

