#include "Widget/Inventory/CQuickSlot.h"
#include "Components/Image.h"
#include "Components/WrapBox.h"
#include "Components/Overlay.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Character/CSurvivor.h"
#include "ActorComponents/CInventoryComponent.h"
#include "Widget/Inventory/CItemDragDropOperation.h"
#include "Widget/Inventory/CItemBase.h"
#include "Widget/Inventory/CInventoryItemSlot.h"
#include "Widget/Inventory/CInventoryPanel.h"
#include "Widget/Inventory/CInventoryPanel_Placeable.h"
#include "Widget/Menu/CInventoryMenu.h"
#include "Widget/CMainHUD.h"
#include "Widget/Chatting/CChattingBox.h"
#include "CGameInstance.h"
#include "Utility/CDebug.h"

FReply UCQuickSlot::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    Super::NativeOnKeyDown(InGeometry, InKeyEvent);
    if (InKeyEvent.GetKey() == EKeys::I)
    {
        ACMainHUD* mainHUD = Cast<ACMainHUD>(GetOwningPlayer()->GetHUD());
        if (mainHUD)
        {
            if (mainHUD->GetSurvivorInventoryWidget()->IsInViewport())
                mainHUD->SetWidgetVisibility(EWidgetCall::CloseWidget);
            else
                mainHUD->SetWidgetVisibility(EWidgetCall::Survivor);
        }

        return FReply::Handled();
    }

    if (InKeyEvent.GetKey() == EKeys::Enter)
    {
        UCGameInstance* gameInstance = Cast<UCGameInstance>(GetGameInstance());
        if (gameInstance)
        {
            if (gameInstance->ChattingBox)
            {
                gameInstance->ChattingBox->SetInputMode();
            }
            else
                CDebug::Print("gameInstance->ChattingBox is not Valid");
        }
        else
            CDebug::Print("gameInstance is not Valid");

        return FReply::Handled();
    }

    return FReply::Unhandled();
}

bool UCQuickSlot::Initialize()
{
	bool Sucess = Super::Initialize();

	if (!Sucess)
		return false;

    SizeBoxes.Add(SizeBox_0);
    SizeBoxes.Add(SizeBox_1);
    SizeBoxes.Add(SizeBox_2);
    SizeBoxes.Add(SizeBox_3);
    SizeBoxes.Add(SizeBox_4);
    SizeBoxes.Add(SizeBox_5);
    SizeBoxes.Add(SizeBox_6);
    SizeBoxes.Add(SizeBox_7);
    SizeBoxes.Add(SizeBox_8);
    SizeBoxes.Add(SizeBox_9);

    ACMainHUD* mainHUD = Cast<ACMainHUD>(GetOwningPlayer()->GetHUD());
    if (mainHUD)
    {
        UUserWidget* inventoryPanel = mainHUD->GetSurvivorInventoryWidget()->GetInventoryPanel();
        if (inventoryPanel)
        {
            UCInventoryPanel* castedInventoryPanel = Cast<UCInventoryPanel>(inventoryPanel);
            if (castedInventoryPanel)
                castedInventoryPanel->QuickSlotReference = this;
        }
    }
	return true;
}

bool UCQuickSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    // 별도로 만든 DragDropOperation 생성 
    const UCItemDragDropOperation* itemDragDrop = Cast<UCItemDragDropOperation>(InOperation);
    if (itemDragDrop && itemDragDrop->SourceItem) // 아이템이 DragDropOperation에서 감지되면 위젯 검사 (아이템이 Drag 중이면)
    {
        CDebug::Print("NativeOnDrop Called", FColor::Cyan);

        if (itemDragDrop->DragStartWidget->IsA(UCInventoryPanel_Placeable::StaticClass()))
        {
            CDebug::Print("Drag From Placeable, Ignore", FColor::Red);
            return false;
        }

        if (bSwapCalled)
            return false;

        if (QuickSlotWrapBox)
        {
            // 드롭된 슬롯의 인덱스
            int32 droppedSlotIndex = INDEX_NONE;
            const FVector2D dropPosition = InDragDropEvent.GetScreenSpacePosition(); // 스크린 좌표
        
            for (int32 i = 0; i < QuickSlotWrapBox->GetChildrenCount(); ++i)
            {
                // 자식 위젯
                UOverlay* overlay = Cast<UOverlay>(QuickSlotWrapBox->GetChildAt(i));
                if (overlay)
                {
                    FGeometry childGeometry = overlay->GetCachedGeometry();
        
                    // ChildGeometry를 기준으로 위치 비교
                    if (childGeometry.IsUnderLocation(dropPosition))
                    {
                        droppedSlotIndex = i;
                        CDebug::Print("droppedSlotIndex", droppedSlotIndex);
                        break;
                    }
                }
                else
                    CDebug::Print("NoSizeBox");
            }
        
            if (droppedSlotIndex != INDEX_NONE)
            {
                if (itemDragDrop->SourceItem->ItemType == EItemType::Hunt)
                {
                    if (itemDragDrop->DragStartWidget->IsA(UCQuickSlot::StaticClass()))
                        ProcessHuntItemInfo(itemDragDrop->SourceItem, droppedSlotIndex, true);
                    else
                        ProcessHuntItemInfo(itemDragDrop->SourceItem, droppedSlotIndex, false);
                }
                else if (itemDragDrop->SourceItem->ItemType == EItemType::Consumable)
                    ProcessConsumableItemInfo(itemDragDrop->SourceItem, droppedSlotIndex);
            }
            else
                CDebug::Print(TEXT("No Dropped Index"));
        }
        
        return true;
    }
    return false;
}

void UCQuickSlot::ProcessHuntItemInfo(class UCItemBase* InItem, int32 InIndex, bool bDragStartFromQuickSlot)
{
    
    CDebug::Print("ProcessHuntItemInfo Called", FColor::Cyan);
    
    UCInventoryItemSlot* itemSlot = CreateWidget<UCInventoryItemSlot>(this, InventorySlotClass);
    itemSlot->SetItemReference(InItem);
    
    if (SizeBoxes[InIndex]->HasAnyChildren() && !bDragStartFromQuickSlot)
    {
        CDebug::Print("ClearChildren");
        //SizeBoxes[InIndex]->ClearChildren();

        ACSurvivor* survivor = Cast<ACSurvivor>(this->GetOwningPlayerPawn());
        if (survivor)
        {
            if (SizeBoxes[InIndex]->HasAnyChildren())
            {
                UCInventoryItemSlot* tempItemSlot = Cast<UCInventoryItemSlot>(SizeBoxes[InIndex]->GetChildAt(0));
                if (tempItemSlot)
                {
                    survivor->GetInventoryComponent()->AddNewItem(tempItemSlot->GetItemReference(), 1);
                    SizeBoxes[InIndex]->ClearChildren();
                }
            }
        }

    }
    SizeBoxes[InIndex]->AddChild(itemSlot);
    
    for (int32 i = 0; i < SizeBoxes.Num(); i++)
    {
        if (!SizeBoxes[i]->HasAnyChildren() || i == InIndex)
            continue;
    
        UCInventoryItemSlot* tempItemSlot = Cast<UCInventoryItemSlot>(SizeBoxes[i]->GetChildAt(0));
    
        if (tempItemSlot->GetItemReference() == itemSlot->GetItemReference())
        {
            if (SizeBoxes[i]->HasAnyChildren())
                SizeBoxes[i]->ClearChildren();
            break;
        }
    }
    
    if (!bDragStartFromQuickSlot)
    {
        ACSurvivor* survivor = Cast<ACSurvivor>(this->GetOwningPlayerPawn());
        if (survivor)
        {
            survivor->GetInventoryComponent()->RemoveSingleItem(InItem);
            survivor->GetInventoryComponent()->OnInventoryUpdated.Broadcast();
        }
    }
}

void UCQuickSlot::ProcessConsumableItemInfo(class UCItemBase* InItem, int32 InIndex)
{
    UCItemBase* createdItem = CreateItem(InItem);
    
    UCInventoryItemSlot* itemSlot = CreateWidget<UCInventoryItemSlot>(this, InventorySlotClass);
    itemSlot->SetItemReference(createdItem);

    if (SizeBoxes[InIndex]->HasAnyChildren())
    {
        CDebug::Print("ClearChildren");
        SizeBoxes[InIndex]->ClearChildren();
    }
    SizeBoxes[InIndex]->AddChild(itemSlot);

    for (int32 i = 0; i < SizeBoxes.Num(); i++)
    {
        if (!SizeBoxes[i]->HasAnyChildren() || i == InIndex)
            continue;
    
        UCInventoryItemSlot* tempItemSlot = Cast<UCInventoryItemSlot>(SizeBoxes[i]->GetChildAt(0));

        if (tempItemSlot->GetItemReference()->ID == itemSlot->GetItemReference()->ID)
        {
            if (SizeBoxes[i]->HasAnyChildren())
                SizeBoxes[i]->ClearChildren();
            break;
        }
    }
}

void UCQuickSlot::RefreshConsumableQuantity()
{
    //CDebug::Print("RefreshConsumableQuantity Called", FColor::White);

    for (int32 i = 0; i < SizeBoxes.Num(); i++)
    {
        if (!SizeBoxes[i]->HasAnyChildren())
            continue;

        UCInventoryItemSlot* tempItemSlot = Cast<UCInventoryItemSlot>(SizeBoxes[i]->GetChildAt(0));

        if (tempItemSlot->GetItemReference()->ItemType == EItemType::Consumable)
        {
            ACSurvivor* survivor = Cast<ACSurvivor>(this->GetOwningPlayerPawn());
            int32 quantity = 0;
            for (TWeakObjectPtr<UCItemBase> itemBase : survivor->GetInventoryComponent()->GetInventoryContents())
            {
                if (itemBase.Get()->ID == tempItemSlot->GetItemReference()->ID)
                    quantity += itemBase.Get()->Quantity;
            }
            CDebug::Print("total Quantity : ", quantity, FColor::White);
            tempItemSlot->GetItemReference()->SetQuantity(quantity);
            tempItemSlot->SetItemQuantityText(quantity);
        }
    }
}

void UCQuickSlot::SwapItemInfo(class UCItemBase* DragStartWidgetItem, class UCItemBase* OwnerWidgetItem)
{
    bSwapCalled = true;
    CDebug::Print("SwapItemInfo Called", FColor::Cyan);
    //if (DragStartWidgetItem->ItemType == EItemType::Consumable && OwnerWidgetItem->ItemType == EItemType::Consumable)
    //    SwapConsumableWithConsumable(DragStartWidgetItem, OwnerWidgetItem);
    //else if (DragStartWidgetItem->ItemType == EItemType::Consumable && OwnerWidgetItem->ItemType == EItemType::Hunt)
    //    SwapConsumableWithHunt(DragStartWidgetItem, OwnerWidgetItem);
    //else if (DragStartWidgetItem->ItemType == EItemType::Hunt && OwnerWidgetItem->ItemType == EItemType::Consumable)
    //    SwapHuntWithConsumable(DragStartWidgetItem, OwnerWidgetItem);
    //else if (DragStartWidgetItem->ItemType == EItemType::Hunt && OwnerWidgetItem->ItemType == EItemType::Hunt)
    //    SwapHuntWithHunt(DragStartWidgetItem, OwnerWidgetItem);

    int32 dragStartItemIndex = INDEX_NONE;
    int32 ownerWidgetItemIndex = INDEX_NONE;

    for (int32 i = 0; i < SizeBoxes.Num(); i++)
    {
        if (!SizeBoxes[i]->HasAnyChildren())
            continue;

        UCInventoryItemSlot* tempItemSlot = Cast<UCInventoryItemSlot>(SizeBoxes[i]->GetChildAt(0));

        if (tempItemSlot->GetItemReference()->ItemType == EItemType::Consumable)
        {
            if (tempItemSlot->GetItemReference()->ID == DragStartWidgetItem->ID)
            {
                SizeBoxes[i]->ClearChildren();
                dragStartItemIndex = i;
            }

            if (tempItemSlot->GetItemReference()->ID == OwnerWidgetItem->ID)
            {
                SizeBoxes[i]->ClearChildren();
                ownerWidgetItemIndex = i;
            }
        }
        else if (tempItemSlot->GetItemReference()->ItemType == EItemType::Hunt)
        {
            if (tempItemSlot->GetItemReference() == DragStartWidgetItem)
            {
                SizeBoxes[i]->ClearChildren();
                dragStartItemIndex = i;
            }

            if (tempItemSlot->GetItemReference() == OwnerWidgetItem)
            {
                SizeBoxes[i]->ClearChildren();
                ownerWidgetItemIndex = i;
            }
        }
    }

    UCInventoryItemSlot* dragStartWidgetItemSlot = CreateWidget<UCInventoryItemSlot>(this, InventorySlotClass);
    dragStartWidgetItemSlot->SetItemReference(OwnerWidgetItem);
    UCInventoryItemSlot* ownerWidgetItemSlot = CreateWidget<UCInventoryItemSlot>(this, InventorySlotClass);
    ownerWidgetItemSlot->SetItemReference(DragStartWidgetItem);

    SizeBoxes[dragStartItemIndex]->AddChild(dragStartWidgetItemSlot);
    SizeBoxes[ownerWidgetItemIndex]->AddChild(ownerWidgetItemSlot);

    bSwapCalled = false;


}

void UCQuickSlot::ProcessDragToInventoryMenu(UCItemBase* InItem)
{
    if (InItem->ItemType == EItemType::Hunt)
    {
        //Drop할 아이템 Spawn Location 과 Transform 설정
        ACSurvivor* survivor = Cast<ACSurvivor>(this->GetOwningPlayerPawn());
        if (!survivor)
            return;

        const FVector SpawnLocation = survivor->GetActorLocation() + (survivor->GetActorForwardVector() * 50.0f);
        const FTransform SpawnTransform(survivor->GetActorRotation(), SpawnLocation);

        //퀵슬롯에서 제거하는 함수
        RemoveQuickSlotItem(InItem);
        survivor->GetInventoryComponent()->OnInventoryUpdated.Broadcast();

        // FItemData ItemToDropData = ItemToDrop->CreateFItemData(ItemToDrop);
        FName ItemID = InItem->ID;
        int32 remainDurability = InItem->ItemStats.RemainDurability;
        if (survivor->HasAuthority())
        {
            survivor->GetInventoryComponent()->PerformDropItem(SpawnTransform, ItemID, 1, remainDurability);
        }
        else
        {
            survivor->GetInventoryComponent()->RequestDropItem(SpawnTransform, ItemID, 1, remainDurability);
        }
    }
    else if (InItem->ItemType == EItemType::Consumable)
        RemoveQuickSlotItem(InItem);
}

void UCQuickSlot::ProcessDragToInventoryPanel(UCItemBase* InItem)
{
    if (InItem->ItemType == EItemType::Hunt)
    {
        ACSurvivor* survivor = Cast<ACSurvivor>(this->GetOwningPlayerPawn());
        if (!survivor)
            return;

        RemoveQuickSlotItem(InItem);
        survivor->GetInventoryComponent()->HandleAddItem(InItem, false);
    }
    else if (InItem->ItemType == EItemType::Consumable)
        RemoveQuickSlotItem(InItem);
}

int32 UCQuickSlot::GetQuickSlotTotalWeight()
{
    return 0;
}

class UCItemBase* UCQuickSlot::CreateItem(class UCItemBase* InItem)
{
    UCItemBase* itemCopy = NewObject<UCItemBase>(UCItemBase::StaticClass());
    itemCopy->ID = InItem->ID;
    ACSurvivor* survivor = Cast<ACSurvivor>(this->GetOwningPlayerPawn());
    int32 quantity = 1;
    if (InItem->ItemType == EItemType::Consumable)
    {
        quantity = 0;
        for (TWeakObjectPtr<UCItemBase> itemBase : survivor->GetInventoryComponent()->GetInventoryContents())
        {
            if (itemBase.Get()->ID == InItem->ID)
                quantity += itemBase.Get()->Quantity;
        }
    }
    itemCopy->Quantity = quantity;
    itemCopy->ItemType = InItem->ItemType;
    itemCopy->TextData = InItem->TextData;
    itemCopy->ItemStats = InItem->ItemStats;
    itemCopy->NumericData = InItem->NumericData;
    itemCopy->AssetData = InItem->AssetData;
    itemCopy->HuntData = InItem->HuntData;
    itemCopy->bIsCopy = true;

    return itemCopy;
}

void UCQuickSlot::RemoveQuickSlotItem(UCItemBase* InItem)
{
    for (int32 i = 0; i < SizeBoxes.Num(); i++)
    {
        if (!SizeBoxes[i]->HasAnyChildren())
            continue;

        UCInventoryItemSlot* tempItemSlot = Cast<UCInventoryItemSlot>(SizeBoxes[i]->GetChildAt(0));

        if (tempItemSlot->GetItemReference() == InItem)
            SizeBoxes[i]->ClearChildren();
    }
}

//void UCQuickSlot::SwapConsumableWithConsumable(class UCItemBase* DragStartWidgetItem, class UCItemBase* OwnerWidgetItem)
//{
//    CDebug::Print("SwapConsumableWithConsumable Called", FColor::Magenta);
//
//    int32 dragStartItemIndex;
//    int32 ownerWidgetItemIndex;
//
//    for (int32 i = 0; i < SizeBoxes.Num(); i++)
//    {
//        if (!SizeBoxes[i]->HasAnyChildren())
//            continue;
//
//        UCInventoryItemSlot* tempItemSlot = Cast<UCInventoryItemSlot>(SizeBoxes[i]->GetChildAt(0));
//
//        if (tempItemSlot->GetItemReference()->ID == DragStartWidgetItem->ID)
//        {
//            SizeBoxes[i]->ClearChildren();
//            dragStartItemIndex = i;
//        }
//
//        if (tempItemSlot->GetItemReference()->ID == OwnerWidgetItem->ID)
//        {
//            SizeBoxes[i]->ClearChildren();
//            ownerWidgetItemIndex = i;
//        }
//    }
//
//    UCInventoryItemSlot* dragStartWidgetItemSlot = CreateWidget<UCInventoryItemSlot>(this, InventorySlotClass);
//    dragStartWidgetItemSlot->SetItemReference(OwnerWidgetItem);
//    UCInventoryItemSlot* ownerWidgetItemSlot = CreateWidget<UCInventoryItemSlot>(this, InventorySlotClass);
//    ownerWidgetItemSlot->SetItemReference(DragStartWidgetItem);
//
//    SizeBoxes[dragStartItemIndex]->AddChild(dragStartWidgetItemSlot);
//    SizeBoxes[ownerWidgetItemIndex]->AddChild(ownerWidgetItemSlot);
//
//    bSwapCalled = false;
//}
//
//void UCQuickSlot::SwapConsumableWithHunt(UCItemBase* DragStartWidgetItem, UCItemBase* OwnerWidgetItem)
//{
//    CDebug::Print("SwapConsumableWithHunt Called", FColor::Magenta);
//
//    int32 dragStartItemIndex;
//    int32 ownerWidgetItemIndex;
//
//    for (int32 i = 0; i < SizeBoxes.Num(); i++)
//    {
//        if (!SizeBoxes[i]->HasAnyChildren())
//            continue;
//
//        UCInventoryItemSlot* tempItemSlot = Cast<UCInventoryItemSlot>(SizeBoxes[i]->GetChildAt(0));
//
//        if (tempItemSlot->GetItemReference()->ID == DragStartWidgetItem->ID)
//        {
//            SizeBoxes[i]->ClearChildren();
//            dragStartItemIndex = i;
//        }
//
//        if (tempItemSlot->GetItemReference() == OwnerWidgetItem)
//        {
//            SizeBoxes[i]->ClearChildren();
//            ownerWidgetItemIndex = i;
//        }
//    }
//
//    UCInventoryItemSlot* dragStartWidgetItemSlot = CreateWidget<UCInventoryItemSlot>(this, InventorySlotClass);
//    dragStartWidgetItemSlot->SetItemReference(OwnerWidgetItem);
//    UCInventoryItemSlot* ownerWidgetItemSlot = CreateWidget<UCInventoryItemSlot>(this, InventorySlotClass);
//    ownerWidgetItemSlot->SetItemReference(DragStartWidgetItem);
//
//    SizeBoxes[dragStartItemIndex]->AddChild(dragStartWidgetItemSlot);
//    SizeBoxes[ownerWidgetItemIndex]->AddChild(ownerWidgetItemSlot);
//
//    bSwapCalled = false;
//}
//
//void UCQuickSlot::SwapHuntWithConsumable(UCItemBase* DragStartWidgetItem, UCItemBase* OwnerWidgetItem)
//{
//    CDebug::Print("SwapHuntWithConsumable Called", FColor::Magenta);
//
//    int32 dragStartItemIndex;
//    int32 ownerWidgetItemIndex;
//
//    for (int32 i = 0; i < SizeBoxes.Num(); i++)
//    {
//        if (!SizeBoxes[i]->HasAnyChildren())
//            continue;
//
//        UCInventoryItemSlot* tempItemSlot = Cast<UCInventoryItemSlot>(SizeBoxes[i]->GetChildAt(0));
//
//        if (tempItemSlot->GetItemReference() == DragStartWidgetItem)
//        {
//            SizeBoxes[i]->ClearChildren();
//            dragStartItemIndex = i;
//        }
//
//        if (tempItemSlot->GetItemReference()->ID == OwnerWidgetItem->ID)
//        {
//            SizeBoxes[i]->ClearChildren();
//            ownerWidgetItemIndex = i;
//        }
//    }
//
//    UCInventoryItemSlot* dragStartWidgetItemSlot = CreateWidget<UCInventoryItemSlot>(this, InventorySlotClass);
//    dragStartWidgetItemSlot->SetItemReference(OwnerWidgetItem);
//    UCInventoryItemSlot* ownerWidgetItemSlot = CreateWidget<UCInventoryItemSlot>(this, InventorySlotClass);
//    ownerWidgetItemSlot->SetItemReference(DragStartWidgetItem);
//
//    SizeBoxes[dragStartItemIndex]->AddChild(dragStartWidgetItemSlot);
//    SizeBoxes[ownerWidgetItemIndex]->AddChild(ownerWidgetItemSlot);
//
//    bSwapCalled = false;
//}
//
//void UCQuickSlot::SwapHuntWithHunt(UCItemBase* DragStartWidgetItem, UCItemBase* OwnerWidgetItem)
//{
//    CDebug::Print("SwapHuntWithHunt Called", FColor::Magenta);
//
//    int32 dragStartItemIndex;
//    int32 ownerWidgetItemIndex;
//
//    for (int32 i = 0; i < SizeBoxes.Num(); i++)
//    {
//        if (!SizeBoxes[i]->HasAnyChildren())
//            continue;
//
//        UCInventoryItemSlot* tempItemSlot = Cast<UCInventoryItemSlot>(SizeBoxes[i]->GetChildAt(0));
//
//        if (tempItemSlot->GetItemReference() == DragStartWidgetItem)
//        {
//            SizeBoxes[i]->ClearChildren();
//            dragStartItemIndex = i;
//        }
//
//        if (tempItemSlot->GetItemReference() == OwnerWidgetItem)
//        {
//            SizeBoxes[i]->ClearChildren();
//            ownerWidgetItemIndex = i;
//        }
//    }
//
//    UCInventoryItemSlot* dragStartWidgetItemSlot = CreateWidget<UCInventoryItemSlot>(this, InventorySlotClass);
//    dragStartWidgetItemSlot->SetItemReference(OwnerWidgetItem);
//    UCInventoryItemSlot* ownerWidgetItemSlot = CreateWidget<UCInventoryItemSlot>(this, InventorySlotClass);
//    ownerWidgetItemSlot->SetItemReference(DragStartWidgetItem);
//
//    SizeBoxes[dragStartItemIndex]->AddChild(dragStartWidgetItemSlot);
//    SizeBoxes[ownerWidgetItemIndex]->AddChild(ownerWidgetItemSlot);
//
//    bSwapCalled = false;
//}

