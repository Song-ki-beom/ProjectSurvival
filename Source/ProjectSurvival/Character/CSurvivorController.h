#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Struct/CItemDataStructures.h"
#include "Perception/AISightTargetInterface.h"
#include "GenericTeamAgentInterface.h"
#include "CSurvivorController.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACSurvivorController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ACSurvivorController();
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamId) override;
protected:
	virtual void BeginPlay() override;

public:
	class UCBuildWidget* GetBuildWidget() { return BuildWidget; }
	void SetupBuildWidget();

private:
	// Init
	void GetSurvivor();
	void SetupInputFunction();
	void UpdateListenerTransform();


	// Build
	void ToggleBuildWidget();
	void SelectQ();
	void SelectW();
	void SelectE(); //Interaction 관련 분기문 존재
	void ReleaseE(); //Interaction 관련 분기문 존재
	void SelectA();
	void SelectS();
	void SelectD();
	void SelectZ();
	void SelectX();
	void SelectC();

	// DoAction + Harvest
	void DoAction();
	void SubAction();
	void HoldAxe();

	//Inventory and Produce
	void ShowWidget();
	//void ToggleProduceWidget();

	//Mouse Event
	void HandleMouseWheelUp();
	void HandleMouseWheelDown();

	//Chatting
	void FocusChattingBox();

	// WorldMap
	void ToggleWorldMap();

	//QuickSlot
	void PressQuickSlot(FKey InPressedKey);

	//BuildInteract
	void ShowBuildInteractWidget();

private:
	class ACSurvivor* Survivor;
	
	FGenericTeamId TeamId;

	bool bIsBuildWidgetOn;
	bool bIsProduceWidgetOn;

	UPROPERTY()
		TSubclassOf<class UUserWidget> BuildWidgetClass;
	UPROPERTY()
		class UCBuildWidget* BuildWidget;

	UPROPERTY()
		TSubclassOf<class UUserWidget> ChattingBoxClass;
	UPROPERTY()
		class UCChattingBox* ChattingBox;

	//UPROPERTY()
	//	TSubclassOf<class UUserWidget> RespawnLocationRegisterClass;
	//UPROPERTY()
	//	class UCRespawnLocationRegist* RespawnLocationRegister;

	FTimerHandle UpdateListenerTransformHandle;

public:
	// 서버 RPC 함수 테스트
	UFUNCTION(Server, Reliable)
		void RequestAddItem(FName ItemID, int32 InQuantity, class ACStructure_Placeable* InPlaceable, FItemNumericData InNumericData, EItemType ItemType, FItemStats InItemStats, EWeaponType InWeaponType);
	UFUNCTION(Server, Reliable)
		void RequestSwapItem(int32 idxBase, int32  idxDrag, class ACStructure_Placeable* InPlaceable);
	UFUNCTION(Server, Reliable)
		void RequestSortInfoWidget(class ACStructure_Placeable* InPlaceable);
	UFUNCTION(Server, Reliable)
		void RequestSplitItem(int32 ItemIdx, int32 AmountToSplit, class ACStructure_Placeable* InPlaceable);
	UFUNCTION(Server, Reliable)
		void RequestCombineItem(int32 idxBase, int32  idxDrag, class ACStructure_Placeable* InPlaceable);
	UFUNCTION(Server, Reliable)
		void RequestRemoveItem(int32 idxRemove,class ACStructure_Placeable* InPlaceable);
	UFUNCTION(Server, Reliable)
		void RequestRemoveAmountOfItem(int32 idxRemove, int32 AmountToRemove, class ACStructure_Placeable* InPlaceable);
	UFUNCTION(Server, Reliable)
		void RequestAddProduceItemToQueue(FName ItemID, class ACStructure_Placeable* InPlaceable);
	UFUNCTION(Server, Reliable)
		void RequestIgnite(class ACStructure_Placeable* InPlaceable);
	UFUNCTION(Server, Reliable)
		void RequestExtinguish(class ACStructure_Placeable* InPlaceable);
	UFUNCTION(Server, Reliable)
		void RequestRemoveProduceItemFromQueue(class ACStructure_Placeable* InPlaceable, int32 InIndex);
	UFUNCTION(Server, Reliable)
		void RequestOpenDoor(class ACStructure_Door* InDoor);
	UFUNCTION(Server, Reliable)
		void RequestCloseDoor(class ACStructure_Door* InDoor);
	UFUNCTION(Server, Reliable)
		void RequestSetRespawnLocationName(class ACStructure_Placeable* InPlaceable, const FText& InText);
	UFUNCTION(NetMulticast, Reliable)
		void BroadcastDestroyPlayerLocation();
	UFUNCTION(Server, Reliable)
		void RequestDestroyPlayerLocation();
	UFUNCTION(Server, Reliable)
		void RequestRepair(int32 ItemIdx, class ACStructure_Placeable* InPlaceable, FName InID, int32 InQuantity, FItemNumericData InNumericData, EItemType InItemType, FItemStats InItemStats, EWeaponType InWeaponType);
	
};

