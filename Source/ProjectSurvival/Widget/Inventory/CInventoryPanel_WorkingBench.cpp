#include "Widget/Inventory/CInventoryPanel_WorkingBench.h"
#include "Widget/Inventory/CInventoryPanel.h"
#include "Widget/Inventory/CItemDragDropOperation.h"
#include "Widget/Inventory/CItemBase.h"
#include "Widget/Inventory/CInventoryItemSlot.h"
#include "Widget/Produce/CProduceWidget.h"
#include "Widget/CMainHUD.h"
#include "ActorComponents/CActorInventoryComponent.h"
#include "Build/CStructure_Placeable.h"
#include "Components/WrapBox.h"
#include "Components/Button.h"
#include "CGameStateBase.h"
#include "Utility/CDebug.h"
#include "Character/CSurvivorController.h"

bool UCInventoryPanel_WorkingBench::Initialize()
{
	bool Sucess = Super::Initialize();

	if (!Sucess)
		return false;

	return true;
}

FReply UCInventoryPanel_WorkingBench::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);
	if (InKeyEvent.GetKey() == EKeys::I)
	{
		ACMainHUD* mainHUD = Cast<ACMainHUD>(this->GetOwningPlayer()->GetHUD());
		if (mainHUD)
			mainHUD->SetWidgetVisibility(EWidgetCall::CloseWidget);
		return FReply::Handled();
	}

	if (InKeyEvent.GetKey() == EKeys::E)
	{
		ACMainHUD* mainHUD = Cast<ACMainHUD>(GetOwningPlayer()->GetHUD());
		if (mainHUD)
			mainHUD->GetProduceWidget()->StartProduce();
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

bool UCInventoryPanel_WorkingBench::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	//별도로 만든 DragDropOperation 생성 
	const UCItemDragDropOperation* ItemDragDrop = Cast<UCItemDragDropOperation>(InOperation);
	if (ItemDragDrop->SourceItem) //아이템이 DragDropOperation에서 감지되면 위젯 검사 (아이템이 Drag중이면 )
	{
		if (ItemDragDrop->DragStartWidget == this)
		{
			return true; // 드래그가 시작된 위젯과 현재 위젯이 같으면 취소
		}
		else
		{
			if (ItemDragDrop->DragStartWidget->GetName().Contains(TEXT("WBP_InventoryPanel")))
			{
				UCInventoryPanel* inventoryPanel = Cast<UCInventoryPanel>(ItemDragDrop->DragStartWidget);
				if (inventoryPanel)
				{
					AddItem(ItemDragDrop->SourceItem, ItemDragDrop->SourceItem->Quantity, OwnerActor);

					inventoryPanel->RemoveItem();
				}
			}
			else
			{
				CDebug::Print("NotContain");
				CDebug::Print(ItemDragDrop->DragStartWidget->GetName());
			}

			
			//CDebug::Print("DragStartWidget is Different");
			return false;
		}
	}
	return false;
}

void UCInventoryPanel_WorkingBench::RefreshWorkingBenchInventory()
{
	//WrapBox에 정보를 추가하기 전에 기존 이미지 삭제
	WorkingBenchInventoryPanel->ClearChildren();

	for (UCItemBase* tempItem : WidgetItems)
	{
		if (tempItem)
		{
			//CDebug::Print("tempItem is Valid", FColor::Magenta);
			UCInventoryItemSlot* ItemSlot = CreateWidget<UCInventoryItemSlot>(this, InventorySlotClass);
			ItemSlot->SetItemReference(tempItem);
			WorkingBenchInventoryPanel->AddChildToWrapBox(ItemSlot);
		}
		else
		{
			CDebug::Print("tempItem is not Valid", FColor::Magenta);
		}
	}
}

void UCInventoryPanel_WorkingBench::RemoveItem(int32 InUniqueItemIndexInWrapBox)
{
	
}

void UCInventoryPanel_WorkingBench::AddItem(class UCItemBase* InItem, const int32 QuantityToAdd, class AActor* InActor)
{
	ACStructure_Placeable* workingBenchActor = Cast<ACStructure_Placeable>(InActor);
	if (workingBenchActor)
	{
		ACSurvivor* survivor = Cast<ACSurvivor>(this->GetOwningPlayerPawn());
		if (survivor)
		{
			// 위젯 클래스에서 RPC함수 호출 불가 + workingBenchActor에서 클라이언트가 RPC함수 호출 불가능해서
			// 클라이언트의 경우 컨트롤러를 통해 RPC를 호출함
			if (survivor->HasAuthority())
				workingBenchActor->PerformAddItem(InItem->ID, QuantityToAdd, InItem->NumericData);
			else
			{
				ACSurvivorController* playerController = Cast<ACSurvivorController>(this->GetOwningPlayer());
				if (playerController)
				{
					playerController->RequestAddItem(InItem->ID, QuantityToAdd, workingBenchActor, InItem->NumericData);
				}
				else
					CDebug::Print("playerController is not valid");
			}
		}
	}
	else
		CDebug::Print("Widget Owner is not Valid");
}