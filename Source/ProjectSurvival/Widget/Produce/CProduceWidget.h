#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CProduceWidget.generated.h"

UCLASS()
class PROJECTSURVIVAL_API UCProduceWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual bool Initialize() override;

public:
	void SetWidgetSwitcherIndex(int32 InIndex);
	void SetProduceDetail(FName InID);
	void RefreshProduceDetail();
	void StartProduce();
	void AddProduceItemToQueue();
	void SetProducingItemText(FText InText);
	//void Test_ShowPlaceableInventory();

private:
	UFUNCTION()
		void ClickBuildStructureButton();
	UFUNCTION()
		void ClickToolButton();
	UFUNCTION()
		void ClickWeaponButton();

private:
	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* ProduceWidgetSwitcher;
	UPROPERTY(meta = (BindWidget))
		class UButton* BuildStructureSelectButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* ToolSelectButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* WeaponSelectButton;
	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* ProducePanelSwitcher;
	UPROPERTY(meta = (BindWidget))
		class UGridPanel* BuildPanel;
	UPROPERTY(meta = (BindWidget))
		class UGridPanel* ToolPanel;
	UPROPERTY(meta = (BindWidget))
		class UGridPanel* WeaponPanel;
	UPROPERTY(meta = (BindWidget))
		class UCProduceDetail* ProduceDetail;
	UPROPERTY(meta = (BindWidget))
		class UWrapBox* ProduceQueue;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ProducingItemText;
	UPROPERTY()
		class UDataTable* ItemData;

	class ACSurvivor* Survivor;
	class UCInventoryComponent* InventoryComponent;
	FName SelectedID;
};
