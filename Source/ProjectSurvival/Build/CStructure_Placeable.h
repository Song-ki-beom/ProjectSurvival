#pragma once

#include "CoreMinimal.h"
#include "Build/CStructure.h"
#include "Widget/CMainHUD.h"
#include "CStructure_Placeable.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnActorInventoryUpdated);

USTRUCT()
struct FItemInformation
{
	GENERATED_BODY()

public:
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

	//void CreateActorInventoryComponent();

	//class UCActorInventoryComponent* GetActorInventoryComponent() { return ActorInventoryComponent; }

	void OpenActorInventory(const class ACSurvivor* Survivor, class AActor* Actor) override;

	//TArray<UCItemBase*> GetActorInventoryContents() { return ActorInventoryContents; }

	//TArray<FName> GetSharedInventoryID() { return SharedInventoryID; }

	FOnActorInventoryUpdated OnActorInventoryUpdated;

	//UFUNCTION()
//	void AddInventoryID(FName InID);

	void PerformAddID(FName InID, int32 InQuantity, FItemNumericData InNumericData, int32 InPlayerIndex);
	void PerformAddID_Client(FName InID, int32 InQuantity, FItemNumericData InNumericData, int32 InPlayerIndex);


	void AddItemInfoToWidget();

	UFUNCTION()
		void OnRep_BroadCastTrigger();
	UFUNCTION()
		void OnRep_AddTrigger();
	UFUNCTION()
		void OnRep_TestTrigger();


	//UFUNCTION(BlueprintCallable, Category = "Inventory")
	//	TArray<FName> GetSharedInventoryID() { return SharedInventoryID; }

private:
	int32 GetIndexOfNonFullStackByID(const FItemInformation InItemInformation);
	bool CheckMaxStack(const FItemInformation InItemInformation, const int32 InIndex);
	void ResetAdditionalInfo();

protected:
	UPROPERTY(EditAnywhere)
		class UBoxComponent* DownBox;

	//UPROPERTY(VisibleAnywhere)
	//	class UCActorInventoryComponent* ActorInventoryComponent;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> ActorInventoryWidgetClass;
	UPROPERTY(EditAnywhere)
		EWidgetCall WidgetCaller;
	UPROPERTY()
		class UUserWidget* ActorInventoryWidget;
	bool bDown_FoundationAndCeilingActorHit;
	bool bCenterHit;
	float PlaceableHeight;

	FRotator CenterRotation;

private:
	UPROPERTY()
		class UCInventoryPanel_WorkingBench* WorkingBenchWidget;
	//UPROPERTY(Replicated)
	//	TArray<FName> SharedInventoryIDArray;
	//UPROPERTY(Replicated)
	//	TArray<int32> SharedInventoryQuantityArray;
	//UPROPERTY(Replicated)
	//	TArray<FItemNumericData> SharedInventoryNumericDataArray;
	UPROPERTY(ReplicatedUsing = OnRep_TestTrigger)
		bool TestTrigger;
	UPROPERTY(ReplicatedUsing = OnRep_BroadCastTrigger)
		bool BroadCastTrigger;
	UPROPERTY(ReplicatedUsing = OnRep_AddTrigger)
		bool ClientAddTrigger;
	UPROPERTY()
		TArray<UCItemBase*> ActorInventoryContents;
	//UPROPERTY()
	//	TArray<FName> InventoryIDArray;
	//UPROPERTY()
	//	TArray<int32> InventoryQuantityArray;
	//UPROPERTY()
	//	TArray<FItemNumericData> InventoryNumericDataArray;

	UPROPERTY(Replicated)
		TArray<FItemInformation> SharedItemInfoArray;
	UPROPERTY()
		TArray<FItemInformation> ItemInfoArray;

	UPROPERTY(Replicated)
		FName AddtionalItemID;
	UPROPERTY(Replicated)
		int32 AddtionalQuantity;
	UPROPERTY(Replicated)
		FItemNumericData AddtionalNumericData;
	UPROPERTY(Replicated)
		int32 AddtionalCallerIndex;
	//UPROPERTY()
	//	class UDataTable* ItemDataTable;


};
