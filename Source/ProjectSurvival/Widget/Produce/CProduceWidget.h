#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget/CMainHUD.h"
#include "CProduceWidget.generated.h"

UENUM()
enum class EOwnerActorType : uint8
{
	Survivor,
	WorkingBench,
	None
};

UCLASS()
class PROJECTSURVIVAL_API UCProduceWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual bool Initialize() override;

public:
	void SetOwnerActor(class ACStructure_Placeable* InActor, EWidgetCall InWidgetCall) { OwnerActor = InActor; WidgetCall = InWidgetCall; }
	void SetProduceWindowName(FText InText);
	void SetButtonVisivility(ESlateVisibility BuildVisibility, ESlateVisibility ToolVisibility, ESlateVisibility WeaponVisibility, ESlateVisibility IgniteVisibility);
	void SetProducePanelSwitcherIndex(int32 InIndex);
	void CreateBuildProduceItemSlot(int32 StartIndex, int32 EndIndex);
	void CreateToolProduceItemSlot(int32 StartIndex, int32 EndIndex);
	void CreateWeaponProduceItemSlot(int32 StartIndex, int32 EndIndex);
	void CreateHarvestProduceItemSlot(int32 StartIndex, int32 EndIndex);
	void CreateConsumableProduceItemSlot(int32 StartIndex, int32 EndIndex);
	void SetProduceDetail(FName InID, int32 InIndex, EWidgetCall InWidgetCall);
	void RefreshProduceDetail();
	void StartProduce();
	void AddProduceItemToQueue(FName InID);
	void SetProducingItemText(FText InText, FLinearColor InLinearColor = FLinearColor::White);
	int32 GetProducePanelSwitcherIndex();
	EWidgetCall GetWidgetCall() { return WidgetCall; }
	class ACStructure_Placeable* GetOwnerActor() { return OwnerActor; }
	void SetIgniteButtonOn();
	void SetIgniteButtonOff();

	class UButton* GetIgniteButton() { return IgniteButton; }
	bool CheckWoodResourceUsed();
	class UWrapBox* GetProduceQueue() { return ProduceQueue; }
	
	void CheckWrapBox(class UWrapBox* InWrapBox);

private:
	UFUNCTION()
		void ClickBuildStructureButton();
	UFUNCTION()
		void ClickToolButton();
	UFUNCTION()
		void ClickWeaponButton();
	UFUNCTION()
		void ClickIgniteButton();


private:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ProduceWindowName;
	UPROPERTY(meta = (BindWidget))
		class UButton* BuildStructureSelectButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* ToolSelectButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* WeaponSelectButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* IgniteButton;
	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* ProducePanelSwitcher;
	UPROPERTY(meta = (BindWidget))
		class UWrapBox* BuildPanel;
	UPROPERTY(meta = (BindWidget))
		class UWrapBox* ToolPanel;
	UPROPERTY(meta = (BindWidget))
		class UWrapBox* WeaponPanel;
	UPROPERTY(meta = (BindWidget))
		class UWrapBox* HarvestPanel;
	UPROPERTY(meta = (BindWidget))
		class UWrapBox* ConsumablePanel;
	UPROPERTY(meta = (BindWidget))
		class UCProduceDetail* ProduceDetail;
	UPROPERTY(meta = (BindWidget))
		class UWrapBox* ProduceQueue;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ProducingItemText;
	UPROPERTY()
		class UCProduceItemSlot* ProduceItemSlotClass;
	UPROPERTY()
		class UDataTable* ItemData;

	EWidgetCall WidgetCall;

	class ACStructure_Placeable* OwnerActor;

	class ACSurvivor* Survivor;
	class UCInventoryComponent* InventoryComponent;

	FName SelectedBuildID;
	FName SelectedToolID;
	FName SelectedWeaponID;
	FName SelectedHarvestID;
	FName SelectedConsumableID;

	bool bIsIgniting = false;
};
