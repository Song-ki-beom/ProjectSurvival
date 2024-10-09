#pragma once

#include "CoreMinimal.h"
#include "Build/CStructure.h"
#include "Widget/CMainHUD.h"
#include "Widget/Build/CRespawnLocationRegist.h"
#include "Struct/CItemDataStructures.h"
#include "CStructure_Placeable.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTextSet, const FText&, InText);

UENUM()
enum class EPlaceableStructureType : uint8
{
	WorkingBench, Furnace, CampFire, Bed, BackPack, RemainBag, None
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
	UPROPERTY()
		FItemStats ItemStats;
	UPROPERTY()
		EWeaponType WeaponType;
};

UCLASS()
class PROJECTSURVIVAL_API ACStructure_Placeable : public ACStructure
{
	GENERATED_BODY()

friend class ACPickUp;

public:
	ACStructure_Placeable();

protected:
	virtual void BeginPlay() override;
	virtual void OpenActorInventory(const class ACSurvivor* Survivor, class AActor* Actor) override;

public:
	void CheckHeight();
	bool GetPlaceableLandScapeHit() { return bLandScapeHit; }
	float GetPlaceableHeight() { return PlaceableHeight; }

	void CheckDown_FoundationAndCeiling();
	bool GetPlaceableDown_FoundationAndCeilingHit() { return bDown_FoundationAndCeilingActorHit; }

	void CheckCenter();
	bool GetPlaceableCenterHit() { return bCenterHit; }

	//Interact
	virtual void DoBuildTypeInteract() override;

	UFUNCTION()
		void SetRespawnLocationText(const FText& InText);

	FOnTextSet OnTextSet;

	//Add
	void PerformAddItem(FName InID, int32 InQuantity, FItemNumericData InNumericData, EItemType InItemType, FItemStats InItemStats, EWeaponType InWeaponType);

	UFUNCTION(NetMulticast, Reliable)
		void BroadcastAddItem(FName InID, int32 InQuantity, FItemNumericData InNumericData, EItemType InItemType, FItemStats InItemStats, EWeaponType WeaponType);



	//Remove
	void PerformRemoveItem(int32 idxRemove);

	UFUNCTION(NetMulticast, Reliable)
		void BroadcastRemoveItem(int32 idxRemove);

	void PerformRemoveAmountOfItem(int32 idxRemove, int32 AmountToRemove);

	UFUNCTION(NetMulticast, Reliable)
		void BroadcastRemoveAmountOfItem(int32 idxRemove, int32 AmountToRemove);

	//Swap
	void PerformSwapItem(int32 idxBase,int32 idxDrag);

	UFUNCTION(NetMulticast, Reliable)
		void BroadcastSwapItem(int32 idxBase, int32 idxDrag);

	//Combine
	void PerformCombineItem(int32 idxBase, int32 idxDrag);

	UFUNCTION(NetMulticast, Reliable)
		void BroadcastCombineItem(int32 idxBase, int32 idxDrag);

	//Sort
	void PerformSortInfoWidget();

	UFUNCTION(NetMulticast, Reliable)
		void BroadcastSortInfoWidget();

	void Merge(TArray<FItemInformation>&  Array, int Left, int Mid, int Right); //분할 후 정복 
	void MergeSort(TArray<FItemInformation>& Array, int Left, int Right); // 분할 정복 Merges Sort 사용 

	//Split
	void PerformSplitItem(int32 ItemIdx, int32 AmountToSplit);

	UFUNCTION(NetMulticast, Reliable)
		void BroadcastSplitItem(int32 ItemIdx, int32 AmountToSplit);

	void AddItemInfoToWidget();

	//PlaceableType
	EPlaceableStructureType GetPlaceableStructureType() { return PlaceableStructureType; }

	//UFUNCTION()
	//	void OnRep_WidgetRefreshTrigger();

	//Produce
	//TArray<FItemInformation> GetItemInfoArray() { return SharedItemInfoArray; }
	TArray<FItemInformation> GetItemInfoArray() { return ItemInfoArray; }

	class UCInventoryPanel_Placeable* GetPlaceableInventoryWidget() { return PlaceableWidget; }
	class UCProduceWidget* GetPlaceableProduceWidget() { return PlaceableProduceWidget; }

	UFUNCTION(NetMulticast, Reliable)
		void BroadcastAddProduceItemToQueue(FName ItemID, class ACStructure_Placeable* InPlaceable);
	UFUNCTION(NetMulticast, Reliable)
		void BroadcastRemoveProduceItemFromQueue(int32 InIndex);

	//Fire
	UFUNCTION(NetMulticast, Reliable)
		void BroadcastIgnite();
	UFUNCTION(NetMulticast, Reliable)
		void BroadcastExtinguish();
	
	void PerformIgnite();
	void PerformExtinguish();

	FSlateBrush GetIgniteButtonNormalBrush() { return IgniteButtonNormalBrush; }
	FSlateBrush GetIgniteButtonPressedBrush() { return IgniteButtonPressedBrush; }

	bool GetIgniteState() { return bIsIgnited; }

	//Bed
	UFUNCTION(NetMulticast, Reliable)
		void BroadcastSetRespawnLocationName(const FText& InText);

	


private:
	int32 GetIndexOfNonFullStackByID(const FItemInformation InItemInformation);
	bool CheckMaxStack(const FItemInformation InItemInformation, const int32 InIndex);
	void CheckWoodResource();

	//Create Placeable Remain Bag
	UFUNCTION()
		void CreatePlaceableRemainBag(class AActor* InDestroyedActor);

protected:
	UPROPERTY(EditAnywhere)
		class UBoxComponent* DownBox;

	bool bLandScapeHit;
	float PlaceableHeight;
	bool bDown_FoundationAndCeilingActorHit;
	bool bCenterHit;
	FRotator CenterRotation;

	UPROPERTY(EditAnywhere)
		EPlaceableStructureType PlaceableStructureType;

	UPROPERTY()
		EPlaceableStructureType BeforePlaceableStructureType;

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
	//UPROPERTY()
	//	class ACSurvivorController* PersonalSurvivorController;

	UPROPERTY()
		class UCInventoryPanel_Placeable* PlaceableWidget;
	//UPROPERTY(ReplicatedUsing = OnRep_WidgetRefreshTrigger)
	//	int32 WidgetRefreshTrigger;
	UPROPERTY()
		TArray<UCItemBase*> ActorInventoryContents;
	//UPROPERTY(Replicated)
	//	TArray<FItemInformation> SharedItemInfoArray;
	UPROPERTY()
		TArray<FItemInformation> ItemInfoArray;
	UPROPERTY()
		class UCProduceWidget* PlaceableProduceWidget;

	bool bIsIgnited = false;

	FSlateBrush IgniteButtonNormalBrush;
	FSlateBrush IgniteButtonPressedBrush;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* FireParticle;
	UPROPERTY()
		class UParticleSystemComponent* SpawnedFireParticle;
	UPROPERTY(EditAnywhere)
		class USoundWave* FireSound;
	UPROPERTY(EditAnywhere)
		class USoundWave* IgniteSound;
	UPROPERTY(EditAnywhere)
		class USoundWave* ExtinguishSound;
	UPROPERTY(EditAnywhere)
		class USoundAttenuation* FireSoundAttenuation;
	UPROPERTY()
		class UAudioComponent* SpawnedFireSound;

	FTimerHandle IgniteTimerHandle;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UCRespawnLocationRegist> RespawnLocationRegistClass;
	UPROPERTY()
		class UCRespawnLocationRegist* RespawnLocationRegistWidget;
};
