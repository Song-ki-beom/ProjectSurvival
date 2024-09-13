#pragma once

#include "CoreMinimal.h"
#include "Build/CStructure.h"
#include "Widget/CMainHUD.h"
#include "Struct/CItemDataStructures.h"
#include "CStructure_Placeable.generated.h"

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

public:
	void CheckDown_FoundationAndCeiling();
	bool GetPlaceableDown_FoundationAndCeilingHit() { return bDown_FoundationAndCeilingActorHit; }

	void CheckCenter();
	bool GetPlaceableCenterHit() { return bCenterHit; }

	float GetPlaceableHeight() { return PlaceableHeight; }

	void OpenActorInventory(const class ACSurvivor* Survivor, class AActor* Actor) override;


	//Add
	void PerformAddItem(FName InID, int32 InQuantity, FItemNumericData InNumericData, EItemType InItemType);
	void PerformRemoveItem(FName InID, int32 InQuantity, FItemNumericData InNumericData);

	//Swap
	void PerformSwapItem(int32 idxBase,int32 idxDrag);

	//Sort
	void PerformSortInfoWidget();
	void Merge(TArray<FItemInformation>&  Array, int Left, int Mid, int Right); //분할 후 정복 
	void MergeSort(TArray<FItemInformation>& Array, int Left, int Right); // 분할 정복 Merges Sort 사용 

	//Split
	void PerformSplitItem(int32 ItemIdx, int32 AmountToSplit);



	void AddItemInfoToWidget();

	UFUNCTION()
		void OnRep_WidgetRefreshTrigger();

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
		TSubclassOf<class UUserWidget> ActorInventoryWidgetClass;
	UPROPERTY(EditAnywhere)
		EWidgetCall WidgetCaller;
	UPROPERTY()
		class UUserWidget* ActorInventoryWidget;

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
};
