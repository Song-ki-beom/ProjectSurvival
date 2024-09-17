// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/CInventoryPanel.h"
#include "Widget/Inventory/CInventoryItemSlot.h"
#include "Widget/Inventory/CInventoryPanel_WorkingBench.h"
#include "Character/CSurvivor.h"
#include "Widget/Inventory/CItemBase.h"
#include "Components/WrapBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "ActorComponents/CInventoryComponent.h"
#include "Widget/Inventory/CItemDragDropOperation.h"
#include "Widget/Inventory/CQuickSlot.h"
#include "Widget/CMainHUD.h"
#include "Utility/CDebug.h"

void UCInventoryPanel::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    PlayerCharacter = Cast<ACSurvivor>(GetOwningPlayerPawn());
    if (PlayerCharacter)
    {
        InventoryReference = PlayerCharacter->GetInventoryComponent();

        if (InventoryReference)
        { //인벤토리의 델리게이트로부터 인벤토리 업데이트 이벤트 바인드
            InventoryReference->OnInventoryUpdated.AddUObject(this, &UCInventoryPanel::RefreshInventory);
            SetInfoText();



        }
    }
}


void UCInventoryPanel::RemoveItem(UCItemBase* ItemToRemove)
{
    InventoryReference->RemoveSingleItem(ItemToRemove);

}

//인벤토리로부터 수량과 용량 정보 업데이트 
void UCInventoryPanel::SetInfoText()
{
    int32 quickSlotWeight = 0;

    if (QuickSlotReference)
    {
        for (USizeBox* sizeBox : QuickSlotReference->GetSizeBoxArray())
        {

            UCInventoryItemSlot* tempItemSlot = Cast<UCInventoryItemSlot>(sizeBox->GetChildAt(0));
            if (tempItemSlot)
                quickSlotWeight += tempItemSlot->GetItemReference()->GetItemSingleWeight();
        }

        CDebug::Print("weight : ", quickSlotWeight, FColor::Magenta);
    }

    WeightInfo->SetText(FText::FromString(FString::SanitizeFloat(InventoryReference->GetInventoryTotalWeight() + quickSlotWeight) + "/" + FString::SanitizeFloat(InventoryReference->GetWeightCapacity())));

    CapacityInfo->SetText(FText::Format(FText::FromString("{0}/{1}"), InventoryReference->GetInventoryContents().Num(), InventoryReference->GetSlotsCapacity()));

}




void UCInventoryPanel::RefreshInventory()
{
    if (InventoryReference && InventorySlotClass)
    {
        //WrapBox에 정보를 추가하기 전에 기존 이미지 삭제
        InventoryPanel->ClearChildren();


        //for 문 Iterate : 
        for (TWeakObjectPtr<UCItemBase> InventoryItem : InventoryReference->GetInventoryContents())
        {
            UCInventoryItemSlot* ItemSlot = CreateWidget<UCInventoryItemSlot>(this, InventorySlotClass);
            ItemSlot->SetItemReference(InventoryItem.Get());
            ItemSlot->SetHUDReference(InventoryReference->GetHUDReference());
            InventoryPanel->AddChildToWrapBox(ItemSlot);

        }
        SetInfoText();


    }

    ACMainHUD* mainHUD = Cast<ACMainHUD>(this->GetOwningPlayer()->GetHUD());
    if (mainHUD)
    {
        UCQuickSlot* quickSlot = Cast<UCQuickSlot>(mainHUD->GetQuickSlotWidget());
        if (quickSlot)
            quickSlot->RefreshConsumableQuantity();
        else
            CDebug::Print("quickSlot is not Valid");
    }
    else
        CDebug::Print("mainHUD is not Valid");
}



bool UCInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    //별도로 만든 DragDropOperation 생성 
    const UCItemDragDropOperation* ItemDragDrop = Cast<UCItemDragDropOperation>(InOperation);
    if (ItemDragDrop->SourceItem && InventoryReference) //아이템이 DragDropOperation에서 감지되면 위젯 검사 (아이템이 Drag중이면 )
    {
        if (ItemDragDrop->DragStartWidget == this)
        {
            CDebug::Print("DragStartWidget is Same");
            return true; // 드래그가 시작된 위젯과 현재 위젯이 같으면 취소
        }

        UCInventoryPanel_WorkingBench* workingBenchPanel = Cast<UCInventoryPanel_WorkingBench>(ItemDragDrop->DragStartWidget);
        if (workingBenchPanel)
        {
            FItemAddResult AddResult = InventoryReference->HandleAddItem(ItemDragDrop->SourceItem);
            if (AddResult.OperationResult == EItemAddResult::NoItemAdded)
            {
                return false;
            }
            else if (AddResult.OperationResult == EItemAddResult::PartialItemAdded)
            {
                workingBenchPanel->RemoveAmountOfItem(ItemDragDrop->SourceItem, AddResult.ActualAmountAdded);
            }
            else if (AddResult.OperationResult == EItemAddResult::AllItemAdded)
            {
                workingBenchPanel->RemoveItem(ItemDragDrop->SourceItem);
            }
            return true;
        }
        else
        {
            ACMainHUD* mainHUD = Cast<ACMainHUD>(GetOwningPlayer()->GetHUD());
            if (mainHUD)
            {
                UUserWidget* quickSlot = Cast<UUserWidget>(mainHUD->GetQuickSlotWidget());
                if (quickSlot)
                {
                    if (ItemDragDrop->DragStartWidget == quickSlot)
                        mainHUD->GetQuickSlotWidget()->ProcessDragToInventoryPanel(ItemDragDrop->SourceItem);
                }
            }
            return true;
        }

    }
    return false;

}

bool UCInventoryPanel::Initialize()
{
    bool Success = Super::Initialize();

    if (IsValid(SortItemButton))
        SortItemButton->OnClicked.AddDynamic(this, &UCInventoryPanel::OnSortItemButtonClicked);

    if (!Success) return false;
    return true;
}

void UCInventoryPanel::OnSortItemButtonClicked()
{
    InventoryReference->SortInventory();
}


