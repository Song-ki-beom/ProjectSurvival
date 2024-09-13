#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Struct/CItemDataStructures.h"
#include "CSurvivorController.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACSurvivorController : public APlayerController
{
	GENERATED_BODY()

public:
	ACSurvivorController();

protected:
	virtual void BeginPlay() override;

public:
	class UCBuildWidget* GetBuildWidget() { return BuildWidget; }
//	void OpenActorInventory(TSubclassOf<class UUserWidget> InClass);


private:
	// Init
	void GetSurvivor();
	void SetupBuildWidget();
	void SetupInputFunction();
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
	// Build TempFunction
	void TestP();

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


private:
	class ACSurvivor* Survivor;

	UPROPERTY()
		TSubclassOf<class UUserWidget> BuildWidgetClass;
	UPROPERTY()
		class UCBuildWidget* BuildWidget;

	//UPROPERTY()
	//	TSubclassOf<class UUserWidget> ProduceWidgetClass;
	//UPROPERTY()
		//class UCProduceWidget* ProduceWidget;

	UPROPERTY()
		TSubclassOf<class UUserWidget> ChattingBoxClass;
	UPROPERTY()
		class UCChattingBox* ChattingBox;

	class UDataTable* BuildStructureData;

	bool bIsBuildWidgetOn;
	bool bIsProduceWidgetOn;

public:
	// 서버 RPC 함수 테스트
	UFUNCTION(Server, Reliable)
		void RequestAddItem(FName ItemID, int32 InQuantity, class ACStructure_Placeable* InPlaceable, FItemNumericData InNumericData, EItemType ItemType);
	UFUNCTION(Server, Reliable)
		void RequestSwapItem(int32 idxBase, int32  idxDrag, class ACStructure_Placeable* InPlaceable);
	UFUNCTION(Server, Reliable)
		void RequestSortInfoWidget(class ACStructure_Placeable* InPlaceable);
	UFUNCTION(Server, Reliable)
		void RequestSplitItem(int32 ItemIdx, int32 AmountToSplit, class ACStructure_Placeable* InPlaceable);

};

