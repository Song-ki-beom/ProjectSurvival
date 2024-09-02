#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CProduceItemSlot.generated.h"

UCLASS()
class PROJECTSURVIVAL_API UCProduceItemSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetProduceSlotIcon(UTexture2D* InTexture2D);

private:
	UPROPERTY(meta = (BindWidget))
		class UImage* ProduceItemSlotIcon;
};
