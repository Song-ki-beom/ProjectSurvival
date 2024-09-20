// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget/Inventory/CItemBase.h"
#include "Widget/Inventory/CInventoryItemSlot.h"
#include "CInventorySubMenu.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnFocusOnSubMenuEnded);
DECLARE_MULTICAST_DELEGATE(FOnUseButtonClicked);
//DECLARE_MULTICAST_DELEGATE(FOnSplitButtonClicked);


UCLASS()
class PROJECTSURVIVAL_API UCInventorySubMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeConstruct() override;
	virtual bool Initialize() override;

protected:

	

	UPROPERTY(VisibleAnywhere, Category = "ItemBase")
		class UCItemBase* ItemReference;
	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot")
		class UCInventoryItemSlot* SlotReference;

	//FORCEINLINE Getter & Setter for ItemReference 
public:
	FORCEINLINE void SetItemReference(class UCItemBase* ItemIn)
	{
		ItemReference = ItemIn;
	}
	FORCEINLINE class UCItemBase* GetItemReference()
	{
		return ItemReference;
	}
	FORCEINLINE void SetSlotReference(class UCInventoryItemSlot* SlotIn)
	{
		SlotReference = SlotIn;
	}
	FORCEINLINE class UCInventoryItemSlot* GetSlotReference()
	{
		return SlotReference;
	}


public:
	UFUNCTION()
	void UpdateSubMenu(ERightClickStartWidget InRightClickStartWidget, bool bIsStackable);

private:
	UFUNCTION()
	void HandleOnUseButtonClicked();
	UFUNCTION()
	void HandleOnBuildButtonClicked();
	UFUNCTION()
	void HandleOnSplitButtonClicked();
	UFUNCTION()
	void HandleOnFinalSplitButtonClicked();
	UFUNCTION()
	void HandleOnCancelButtonClicked();

public:
	FOnFocusOnSubMenuEnded OnFocusOnSubMenuEnded;
	FOnUseButtonClicked OnUseButtonClicked;
	//FOnSplitButtonClicked OnSplitButtonClicked;

	UPROPERTY(meta = (BindWidget))
		class UButton* ActionButton;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ActionText;
	UPROPERTY(meta = (BindWidget))
		class UButton* SplitButton;
	UPROPERTY(meta = (BindWidget))
		class USizeBox* SubMenuSizeBox;
	UPROPERTY(meta = (BindWidget))
		class USizeBox* SplitMenuSizeBox;
	UPROPERTY(meta = (BindWidget))
		class UButton* FinalSplitButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* CancelButton;
	UPROPERTY(meta = (BindWidget))
		class UEditableText* SplitQuantityEditText;

private:
	EItemType TextItemType;
	bool bBeforeHideActionButton = false;





};
