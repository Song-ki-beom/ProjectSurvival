// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/CInventoryPanel.h"
#include "Widget/Inventory/CInventoryItemSlot.h"
#include "Character/CSurvivor.h"
#include "Widget/Inventory/CItemBase.h"
#include "Components/WrapBox.h"
#include "Components/TextBlock.h"
#include "ActorComponents/CInventoryComponent.h"


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


//인벤토리로부터 수량과 용량 정보 업데이트 
void UCInventoryPanel::SetInfoText() 
{
    WeightInfo->SetText(FText::Format(FText::FromString("{0}/{1}"), InventoryReference->GetInventoryTotalWeight(), InventoryReference->GetWeightCapacity()));

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
            UCInventoryItemSlot* ItemSlot = CreateWidget<UCInventoryItemSlot>(this,InventorySlotClass);

            ItemSlot->SetItemReference(InventoryItem.Get());
            InventoryPanel->AddChildToWrapBox(ItemSlot);

        }
        SetInfoText();


    }
}



bool UCInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    return false;
}
