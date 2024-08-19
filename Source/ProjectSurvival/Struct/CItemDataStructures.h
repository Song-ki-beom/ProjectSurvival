#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CItemDataStructures.generated.h"

UENUM()
enum class EItemType : uint8
{
	Harvest UMETA(DisplayName = "Harvest"),
	Build UMETA(DisplayName = "Build"),
	Hunt UMETA(DisplayName = "Hunt"),
};

USTRUCT()
struct FItemStats
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere)
		float Durability;
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

};