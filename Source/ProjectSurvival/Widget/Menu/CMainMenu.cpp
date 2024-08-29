// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Menu/CMainMenu.h"
#include "Character/CSurvivor.h"
#include "Widget/Inventory/CItemDragDropOperation.h"
#include "ActorComponents/CInventoryComponent.h"
#include "Widget/Inventory/CItemBase.h"
#include "Input/Reply.h"

void UCMainMenu::NativeOnInitialized() 
{

	Super::NativeOnInitialized();
}

void UCMainMenu::NativeConstruct()
{

	Super::NativeConstruct();

	PlayerCharacter = Cast<ACSurvivor>(GetOwningPlayerPawn());

}

FReply UCMainMenu::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);
	if (InKeyEvent.GetKey() == EKeys::I)
	{
		OnMainMenuToggled.Broadcast();  // 메뉴를 켜고 끄는 함수 호출
		return FReply::Handled();  // 입력을 처리했다고 반환
	}
	return FReply::Unhandled();

}


bool UCMainMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{


	//별도로 만든 DragDropOperation 생성 
	const UCItemDragDropOperation* ItemDragDrop = Cast<UCItemDragDropOperation>(InOperation);
	if (PlayerCharacter && ItemDragDrop->SourceItem) // 해당 UI 내에서 떨어뜨릴 아이템이 감지되면 
	{
		if (ItemDragDrop->SourceItem->ItemType == EItemType::Build) // 빌드타입 아이템 드랍 금지 
			return true;

		PlayerCharacter->GetInventoryComponent()->DropItem(ItemDragDrop->SourceItem, ItemDragDrop->SourceItem->Quantity);
		return true; 
	}
	return false;
}