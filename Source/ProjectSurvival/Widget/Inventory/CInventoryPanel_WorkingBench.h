// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CInventoryPanel_WorkingBench.generated.h"



UCLASS()
class PROJECTSURVIVAL_API UCInventoryPanel_WorkingBench : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	void SetOwnerActor(class AActor* InActor) { OwnerActor = InActor; }
	//class AActor* GetOwnerActor() { return OwnerActor; }

	UFUNCTION()
		void RefreshWorkingBenchInventory();

private:
	UFUNCTION()
		void ChangeButtonColor();

	void AddItem(class UCItemBase* InItem, const int32 QuantityToAdd, class AActor* InActor);


private:
	UPROPERTY(meta = (BindWidget))
		class UWrapBox* WorkingBenchInventoryPanel;
	UPROPERTY(meta = (BindWidget))
		class UButton* TestButton;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UCInventoryItemSlot> InventorySlotClass;

	TArray<UCItemBase*> ItemArray;

	class AActor* OwnerActor;
};
