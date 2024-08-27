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



protected:
	void SetInfoText();
	virtual void NativeOnInitialized() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;






public:
	UPROPERTY(meta=(BindWidget))
	class UWrapBox* InventoryPanel;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WeightInfo;


	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CapacityInfo;


	UPROPERTY()
	class ACSurvivor* PlayerCharacter;

	UPROPERTY()
	class UCInventoryComponent* InventoryReference;


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCInventoryItemSlot> InventorySlotClass;
};
