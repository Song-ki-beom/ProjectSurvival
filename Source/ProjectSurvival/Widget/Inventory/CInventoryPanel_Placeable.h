// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget/Inventory/CItemBase.h"
#include "CInventoryPanel_Placeable.generated.h"

UCLASS()
class PROJECTSURVIVAL_API UCInventoryPanel_Placeable : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	void SetOwnerActor(class ACStructure_Placeable* InActor) { OwnerActor = InActor; }
	class ACStructure_Placeable* GetOwnerActor() { return OwnerActor; }

	void SetInventoryWindowName(FText InText);

	UFUNCTION()
		void RefreshPlaceableInventory();
	UFUNCTION()
		void OnSortInventoryClicked();


	void SetWidgetItems(TArray<UCItemBase*> InArray);
	

	void RemoveItem(class UCItemBase* ItemToRemove);
	void RemoveAmountOfItem(class UCItemBase* ItemToRemove, int32 AmountToRemove);
	bool CombineItem(class UCItemBase* ItemOnBase, class UCItemBase* ItemFromDrag);
	void SwapItem(class UCItemBase* ItemOnBase, class UCItemBase* ItemFromDrag);
	bool SplitExistingStack(class UCItemBase* ItemIn, int32 AmountToSplit);

	TArray<UCItemBase*> GetWidgetItems() { return WidgetItems; }

	void AddItem(class UCItemBase* InItem, const int32 QuantityToAdd, class AActor* InActor);

	void RepairItem(class UCItemBase* ItemOnBase);

private:
	int32 FindItemIndex(class UCItemBase* Item);
	//함수를 객체화하여 생산성 높임 
	bool PerformActionIfHasAuthority(TFunction<void(class ACStructure_Placeable*)> ServerAction, TFunction<void(class ACSurvivorController*, class ACStructure_Placeable*)> ClientAction);


private:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* InventoryWindowName;
	UPROPERTY(meta = (BindWidget))
		class UWrapBox* PlaceableInventoryPanel;
	UPROPERTY(meta = (BindWidget))
		class UButton* SortItemButton;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UCInventoryItemSlot> InventorySlotClass;
	UPROPERTY()
		TArray<UCItemBase*> WidgetItems;

	class ACStructure_Placeable* OwnerActor;
};
