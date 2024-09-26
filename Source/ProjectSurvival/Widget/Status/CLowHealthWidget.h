#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CLowHealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCLowHealthWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void ShowLowHealthEffect();
	UFUNCTION()
	void HideLowHealthEffect();

public:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* LowHealthAnim;

private:
	class UCStatusComponent* StatusComponent;
	FTimerHandle LowHealthTimerHandle;
};
