#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
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


private:
	class ACSurvivor* Survivor;

	UPROPERTY()
		TSubclassOf<class UUserWidget> BuildWidgetClass;
	UPROPERTY()
		class UCBuildWidget* BuildWidget;

	UPROPERTY()
		TSubclassOf<class UUserWidget> ProduceWidgetClass;
	UPROPERTY()
		class UCProduceWidget* ProduceWidget;

	class UDataTable* BuildStructureData;

	bool bIsBuildWidgetOn;
	bool bIsProduceWidgetOn;
};

