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

	UPROPERTY(EditAnywhere, Category = "Item")
		int32 Quantity; //스택 수 같은 아이템 항목에서 개수를 분리하는 등의 액션을 취하기 위해 따로 수량 변수 사용 (ex.) 나무 뗄감 x3 에서 한개 떼어서 버릴때)
	UPROPERTY(EditAnywhere, Category = "Item")
		FName ID; //데이터 테이블에서 아이템을 인식하는데 필요한 고유 식별 수단
	UPROPERTY(EditAnywhere, Category = "Item")
		EItemType ItemType; // 해당 아이템의 속성을 구분하는 Enum
	UPROPERTY(EditAnywhere, Category = "Item")
		FItemStats ItemStats; // DamageValue, 내구도 등 아이템의 Status 관련 수치를 모아둔 Struct
	UPROPERTY(EditAnywhere, Category = "Item")
		FItemTextData TextData; // 툴팁이나 월드상에서의 상호작용 UI 에 표시할 FText 를 모아둔 Struct
	UPROPERTY(EditAnywhere, Category = "Item")
		FItemNumericData NumericData; // 무게 , 슬롯 당 스택 사이즈 등 인벤토리와의 상호작용할때 필요한 수치를 모아둔 Struct
	UPROPERTY(EditAnywhere, Category = "Item")
		FItemAssetData AssetData; // 슬롯 UI에 표시되는 아이콘 Texture, 월드 상에 보여지는 Mesh 를 모아둔 Struct
	UPROPERTY(EditAnywhere, Category = "Item")
		FProduceWidgetData ProduceData; //아이템 제작에 필요한 정보를 모아둔 Struct 
	UPROPERTY(EditAnywhere, Category = "Item")
		FBuildData BuildData; // 건축물(Container) 빌드에 필요한 정보를 모아둔 Struct
	UPROPERTY(EditAnywhere, Category = "Item")
		FHuntData HuntData; // ItemType 이 Hunt(무기)일 경우, 해당 무기의 WeaponType을 저장해둔 데이터 

	bool bIsCopy;
	bool bIsPickup;
	bool bIsSplit;
	bool bIsDropMesh = false;

};

