#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Net/UnrealNetwork.h"
#include "CItemDataStructures.generated.h"

UENUM()
enum class EItemType : uint8
{
	Harvest UMETA(DisplayName = "Harvest"),
	Build UMETA(DisplayName = "Build"),
	Hunt UMETA(DisplayName = "Hunt"),
	Consumable UMETA(DisplayName = "Consumable"),
	Container UMETA(DisplayName = "Container"),
	NoType UMETA(DisplayName = "NoType")
};

USTRUCT()
struct FItemStats
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere)
		int32 RemainDurability;
	UPROPERTY(EditAnywhere)
		int32 MaxDurability;
	UPROPERTY(EditAnywhere)
		float DamageValue;
	UPROPERTY(EditAnywhere)
		float SellValue;
};

USTRUCT()
struct FItemTextData
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(EditAnywhere)
	FText Description;

	UPROPERTY(EditAnywhere)
	FText InteractionText; // 픽업 시 상호작용 텍스트 ex) 줍기

	UPROPERTY(EditAnywhere)
	FText UsageText; // 인벤토리에서의 상호작용 텍스트 ex) 인벤토리에서 포션 drink
};

USTRUCT()
struct FItemNumericData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
		float Weight;
	UPROPERTY(EditAnywhere)
		int32 MaxStackSize;
	UPROPERTY(EditAnywhere)
		bool bIsStackable;
};

USTRUCT()
struct FItemAssetData 
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere)
		UTexture2D* Icon;
	UPROPERTY(EditAnywhere)
		UStaticMesh* Mesh;
	UPROPERTY(EditAnywhere)
		UStaticMesh* DropMesh;
};

USTRUCT()
struct FProduceSlotData :public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
		FName ResourceID;
	UPROPERTY(EditAnywhere)
		UTexture2D* ResourceIcon;
	UPROPERTY(EditAnywhere)
		FText ResourceName;
	UPROPERTY(EditAnywhere)
		int32 ProduceResourceDemand;
};

USTRUCT()
struct FProduceWidgetData :public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
		bool bApplyRichText;
	UPROPERTY(EditAnywhere)
		float ProduceTime;
	UPROPERTY(EditAnywhere)
		FText ProduceMethod;
	UPROPERTY(EditAnywhere)
		FText FlavorText;
	UPROPERTY(EditAnywhere)
		FProduceSlotData ProduceResource_1;
	UPROPERTY(EditAnywhere)
		FProduceSlotData ProduceResource_2;
	UPROPERTY(EditAnywhere)
		FProduceSlotData ProduceResource_3;
	UPROPERTY(EditAnywhere)
		FProduceSlotData ProduceResource_4;
	UPROPERTY(EditAnywhere)
		FProduceSlotData ProduceResource_5;
};

UENUM()
enum class EInteractableBuildType : uint8
{
	Door,
	None
};

USTRUCT()
struct FBuildData :public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
		bool bIsInteractableBuildStructure;
	UPROPERTY(EditAnywhere)
		FText BuildInteractText;
	UPROPERTY(EditAnywhere)
		EInteractableBuildType InteractableBuildType;
	UPROPERTY()
		bool bIsDoorOpened = false;
};

USTRUCT()
struct FItemData :public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
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
	UPROPERTY(EditAnywhere, Category = "Item Data")
		FProduceWidgetData ProduceData;
	UPROPERTY(EditAnywhere, Category = "Item Data")
		FBuildData BuildData;
};

