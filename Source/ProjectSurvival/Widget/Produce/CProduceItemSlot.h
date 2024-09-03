#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CProduceItemSlot.generated.h"

UCLASS()
class PROJECTSURVIVAL_API UCProduceItemSlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

public:
	void SetProduceSlotIcon(UTexture2D* InTexture2D);
	void SetProduceSlotID(FName InID);

	UFUNCTION()
		void ClickProduceSlotButton();

private:
	UPROPERTY(meta = (BindWidget))
		class UImage* ProduceItemSlotIcon;
	UPROPERTY(meta = (BindWidget))
		class UButton* ProduceItemSlotButton;

	FName ItemID;
};
