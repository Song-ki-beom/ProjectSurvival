#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/SizeBox.h"
#include "CQuickSlot.generated.h"

UCLASS()
class PROJECTSURVIVAL_API UCQuickSlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual bool Initialize() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	void ProcessHuntItemInfo(class UCItemBase* InItem, int32 InIndex, bool bDragStartFromQuickSlot);
	void ProcessConsumableItemInfo(class UCItemBase* InItem, int32 InIndex);
	void RefreshConsumableQuantity();
	void SwapItemInfo(class UCItemBase* DragStartWidgetItem, class UCItemBase* OwnerWidgetItem);
	void ProcessDragToInventoryMenu(UCItemBase* InItem);
	void ProcessDragToInventoryPanel(UCItemBase* InItem);
	int32 GetQuickSlotTotalWeight();

	TArray<USizeBox*> GetSizeBoxArray() { return SizeBoxes; }

private:
	class UCItemBase* CreateItem(class UCItemBase* InItem);
	void RemoveQuickSlotItem(class UCItemBase* InItem);
	//void SwapConsumableWithConsumable(class UCItemBase* DragStartWidgetItem, class UCItemBase* OwnerWidgetItem);
	//void SwapConsumableWithHunt(class UCItemBase* DragStartWidgetItem, class UCItemBase* OwnerWidgetItem);
	//void SwapHuntWithConsumable(class UCItemBase* DragStartWidgetItem, class UCItemBase* OwnerWidgetItem);
	//void SwapHuntWithHunt(class UCItemBase* DragStartWidgetItem, class UCItemBase* OwnerWidgetItem);


private:
	UPROPERTY(meta = (BindWidget))
		class UWrapBox* QuickSlotWrapBox;
	UPROPERTY(meta = (BindWidget))
		class USizeBox* SizeBox_0;
	UPROPERTY(meta = (BindWidget))
		class USizeBox* SizeBox_1;
	UPROPERTY(meta = (BindWidget))
		class USizeBox* SizeBox_2;
	UPROPERTY(meta = (BindWidget))
		class USizeBox* SizeBox_3;
	UPROPERTY(meta = (BindWidget))
		class USizeBox* SizeBox_4;
	UPROPERTY(meta = (BindWidget))
		class USizeBox* SizeBox_5;
	UPROPERTY(meta = (BindWidget))
		class USizeBox* SizeBox_6;
	UPROPERTY(meta = (BindWidget))
		class USizeBox* SizeBox_7;
	UPROPERTY(meta = (BindWidget))
		class USizeBox* SizeBox_8;
	UPROPERTY(meta = (BindWidget))
		class USizeBox* SizeBox_9;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UCInventoryItemSlot> InventorySlotClass;
	UPROPERTY()
		TArray<USizeBox*> SizeBoxes;

	bool bSwapCalled = false;
};
