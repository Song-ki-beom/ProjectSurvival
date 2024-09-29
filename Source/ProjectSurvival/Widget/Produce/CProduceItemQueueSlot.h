#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Struct/CItemDataStructures.h"
#include "CProduceItemQueueSlot.generated.h"

UCLASS()
class PROJECTSURVIVAL_API UCProduceItemQueueSlot : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;

public:
	void SetProduceItemID(FName InID);
	void SetProduceQueueSlotIcon(UTexture2D* InTexture2D);
	void SetProduceTimeText(FText InText);
	void SetProduceItemName(FText InText);
	void SetProduceWidgetData(FProduceWidgetData InProduceWidgetData);
	void InitProduce();
	void StartProduce();
	void SetProduceProgress();
	void PauseProduceProgress();
	void EndProduce();
	void CheckWrapBox(class UWrapBox* InWrapBox);
	void RemoveProduceItemQueueSlotWidget();

private:
	UFUNCTION()
		void CancleProduce();

private:
	UPROPERTY(meta = (BindWidget))
		class UImage* ProduceItemQueueSlotIcon;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* ProduceProgressBar;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ProduceTimeText;
	UPROPERTY(meta = (BindWidget))
		class UButton* ProduceCancleButton;

	UPROPERTY()
		class UCProduceWidget* ProduceWidget;

	bool bIsInitialized = false;
	FName ProduceItemID;
	FText ProduceItemName;
	FProduceWidgetData ProduceWidgetData;
	bool bIsProducing = false;
	FTimerHandle ProgressTimerHandle;
	FTimerHandle PauseProgressTimerHandle;
	float TotalProduceTime;
	float RemainProduceTime;
	bool bIsPlaceableOwner = false;

	UPROPERTY()
		class ACSurvivor* Survivor;
	UPROPERTY()
		class ACStructure_Placeable* IgniteUsingPlaceable;
	UPROPERTY()
		class UCItemBase* ProduceTargetItem;
};
