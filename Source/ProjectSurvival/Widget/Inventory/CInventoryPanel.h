// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CInventoryPanel.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCInventoryPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void RefreshInventory();

	void RemoveItem(class UCItemBase* ItemToRemove);



protected:
	void SetInfoText();
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override; 
	//	virtual FReply  NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual bool Initialize() override;

private:
	UFUNCTION()
	void OnSortItemButtonClicked();




public:
	UPROPERTY(meta=(BindWidget))
	class UWrapBox* InventoryPanel;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WeightInfo;


	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CapacityInfo;

	UPROPERTY(meta = (BindWidget))
		class UButton* SortItemButton;


	UPROPERTY()
	class ACSurvivor* PlayerCharacter;

	UPROPERTY()
	class UCInventoryComponent* InventoryReference;

	UPROPERTY()
	class UCQuickSlot* QuickSlotReference;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCInventoryItemSlot> InventorySlotClass;
};
