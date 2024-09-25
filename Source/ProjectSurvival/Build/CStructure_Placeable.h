#pragma once

#include "CoreMinimal.h"
#include "Build/CStructure.h"
#include "Widget/CMainHUD.h"
#include "Struct/CItemDataStructures.h"
#include "CStructure_Placeable.generated.h"

UENUM()
enum class EPlaceableStructureType : uint8
{
	WorkingBench, Furnace, None
};

USTRUCT()
struct FItemInformation
{
	GENERATED_BODY()

public:
	UPROPERTY()
		EItemType ItemType;
	UPROPERTY()
		FName ItemID;

	UPROPERTY()
		int32 Quantity;

	UPROPERTY()
		FItemNumericData NumericData;
};

UCLASS()
class PROJECTSURVIVAL_API ACStructure_Placeable : public ACStructure
{
	GENERATED_BODY()

public:
	ACStructure_Placeable();

protected:
	virtual void BeginPlay() override;
	virtual void OpenActorInventory(const class ACSurvivor* Survivor, class AActor* Actor) override;

public:
	void CheckDown_FoundationAndCeiling();
	bool GetPlaceableDown_FoundationAndCeilingHit() { return bDown_FoundationAndCeilingActorHit; }

	void CheckCenter();
	bool GetPlaceableCenterHit() { return bCenterHit; }

	float GetPlaceableHeight() { return PlaceableHeight; }

	//Add
	void PerformAddItem(FName InID, int32 InQuantity, FItemNumericData InNumericData, EItemType InItemType);
	//Remove
	void PerformRemoveItem(int32 idxRemove);
	void PerformRemoveAmountOfItem(int32 idxRemove, int32 AmountToRemove);
	//Swap
	void PerformSwapItem(int32 idxBase,int32 idxDrag);

	//Combine
	void PerformCombineItem(int32 idxBase, int32 idxDrag);

	//Sort
	void PerformSortInfoWidget();
	void Merge(TArray<FItemInformation>&  Array, int Left, int Mid, int Right); //분할 후 정복 
	void MergeSort(TArray<FItemInformation>& Array, int Left, int Right); // 분할 정복 Merges Sort 사용 

	//Split
	void PerformSplitItem(int32 ItemIdx, int32 AmountToSplit);

	
	



	void AddItemInfoToWidget();

	UFUNCTION()
		void OnRep_WidgetRefreshTrigger();

	//Produce
	TArray<FItemInformation> GetItemInfoArray() { return SharedItemInfoArray; }

	class UCInventoryPanel_WorkingBench* GetWorkingBenchInventory() { return WorkingBenchWidget; }
	class UCProduceWidget* GetWorkingBenchProduceWidget() { return WorkingBenchProduceWidget; }

	UFUNCTION(NetMulticast, Reliable)
		void BroadcastAddProduceItemToQueue(FName ItemID, class ACStructure_Placeable* InPlaceable);

private:
	int32 GetIndexOfNonFullStackByID(const FItemInformation InItemInformation);
	bool CheckMaxStack(const FItemInformation InItemInformation, const int32 InIndex);

protected:
	UPROPERTY(EditAnywhere)
		class UBoxComponent* DownBox;
	bool bDown_FoundationAndCeilingActorHit;
	bool bCenterHit;
	float PlaceableHeight;
	FRotator CenterRotation;

	UPROPERTY(EditAnywhere)
		EPlaceableStructureType PlaceableStructureType;

	UPROPERTY(EditAnywhere)
		EWidgetCall WidgetCaller;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> ActorInventoryWidgetClass;
	UPROPERTY()
		class UUserWidget* ActorInventoryWidget;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> ActorProduceWidgetClass;
	UPROPERTY()
		class UUserWidget* ActorProduceWidget;


private:
	UPROPERTY()
		class UCInventoryPanel_WorkingBench* WorkingBenchWidget;
	UPROPERTY(ReplicatedUsing = OnRep_WidgetRefreshTrigger)
		int32 WidgetRefreshTrigger;
	UPROPERTY()
		TArray<UCItemBase*> ActorInventoryContents;
	UPROPERTY(Replicated)
		TArray<FItemInformation> SharedItemInfoArray;
	UPROPERTY()
		TArray<FItemInformation> ItemInfoArray;
	UPROPERTY()
		class UCProduceWidget* WorkingBenchProduceWidget;
};
