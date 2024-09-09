// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/CEarnInfoPanel.h"


void UCEarnInfoPanel::AddEarnedItemSlot(UObject* Item)
{
	EarnedItemList->AddItem(Item);
    // 타이머를 생성하여 특정 시간이 지난 후 아이템을 삭제하도록 설정
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &UCEarnInfoPanel::DeleteEarnedItemSlot, Item), 5.0f, false);

    // TMap에 아이템과 타이머 핸들을 저장
    ItemTimers.Add(Item, TimerHandle);
}

void UCEarnInfoPanel::DeleteEarnedItemSlot(UObject* Item)
{
	EarnedItemList->RemoveItem(Item);

}
