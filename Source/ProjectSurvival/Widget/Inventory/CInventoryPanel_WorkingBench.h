// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget/Inventory/CItemBase.h"
#include "CInventoryPanel_WorkingBench.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnWorkingBenchUpdated);

UCLASS()
class PROJECTSURVIVAL_API UCInventoryPanel_WorkingBench : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	void SetOwnerActor(class ACStructure_Placeable* InActor) { OwnerActor = InActor; }
	//class AActor* GetOwnerActor() { return OwnerActor; }

	UFUNCTION()
		void RefreshWorkingBenchInventory();

//	TArray<UCItemBase*> GetWidgetItems() { return  WidgetItems; }
	void SetWidgetItems(TArray<UCItemBase*> InArray) { WidgetItems = InArray; }

	FOnWorkingBenchUpdated OnWorkingBenchUpdated;

private:
	void AddItem(class UCItemBase* InItem, const int32 QuantityToAdd, class AActor* InActor);


private:
	UPROPERTY(meta = (BindWidget))
		class UWrapBox* WorkingBenchInventoryPanel;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UCInventoryItemSlot> InventorySlotClass;
	UPROPERTY()
		TArray<UCItemBase*> WidgetItems;

	//TArray<UCItemBase*> ItemArray;

	class ACStructure_Placeable* OwnerActor;
};
