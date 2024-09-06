#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
	void StartProduce();
	void SetProduceProgress();
	void EndProduce();
	void CheckWrapBox(class UWrapBox* InWrapBox);

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

	class UCProduceWidget* ProduceWidget;

	FName ProduceItemID;
	FText ProduceItemName;
	bool bIsWaiting = true;
	FTimerHandle ProgressTimerHandle;
	float TotalProduceTime;
	float RemainProduceTime;
};
