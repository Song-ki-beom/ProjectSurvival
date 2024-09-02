#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CProduceWidget.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnProduceWidgetToggled);

UCLASS()
class PROJECTSURVIVAL_API UCProduceWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual bool Initialize() override;

public:
	FOnProduceWidgetToggled OnProduceWidgetToggled;

private:
	UFUNCTION()
		void ClickBuildStructureButton();
	UFUNCTION()
		void ClickToolButton();
	UFUNCTION()
		void ClickWeaponButton();

private:
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
	UPROPERTY()
		class UDataTable* ItemData;
};
