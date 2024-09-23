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

	if (IsValid(SortItemButton))
		SortItemButton->OnClicked.AddDynamic(this, &UCInventoryPanel_WorkingBench::OnSortInventoryClicked);

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
			UCInventoryPanel* inventoryPanel = Cast<UCInventoryPanel>(ItemDragDrop->DragStartWidget);
			if (inventoryPanel)
			{
				AddItem(ItemDragDrop->SourceItem, ItemDragDrop->SourceItem->Quantity, OwnerActor); //공유 인벤토리에 아이템 추가 
				inventoryPanel->RemoveItem(ItemDragDrop->SourceItem); //개인 인벤토리 아이템 제거 
			}
			else
			{
				CDebug::Print("NotContain");
				CDebug::Print(ItemDragDrop->DragStartWidget->GetName());
			}
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


bool UCInventoryPanel_WorkingBench::CombineItem(UCItemBase* ItemOnBase, UCItemBase* ItemFromDrag)
{
		if (ItemOnBase->IsFullItemStack() || ItemFromDrag->IsFullItemStack()) //둘중에 하나가 풀스택이면 Swap Item 
		{
			SwapItem(ItemOnBase, ItemFromDrag);
			return true;
		}
	
		int32 idxBase = FindItemIndex(ItemOnBase);
		int32 idxDrag = FindItemIndex(ItemFromDrag);

		
		return PerformActionIfHasAuthority(
			// Server
			[=](ACStructure_Placeable* workingBenchActor)
			{
				workingBenchActor->PerformCombineItem(idxBase, idxDrag);
			},
			// Client
				[=](ACSurvivorController* playerController, ACStructure_Placeable* workingBenchActor)
			{
				playerController->RequestCombineItem(idxBase, idxDrag, workingBenchActor);
			}
			);

	
}


void UCInventoryPanel_WorkingBench::RemoveItem(UCItemBase* ItemToRemove)
{
	int32 idxRemove = FindItemIndex(ItemToRemove);
	PerformActionIfHasAuthority(
		// Server
		[=](ACStructure_Placeable* workingBenchActor)
		{
			workingBenchActor->PerformRemoveItem(idxRemove);
		},
		// Client
			[=](ACSurvivorController* playerController, ACStructure_Placeable* workingBenchActor)
		{
			playerController->RequestRemoveItem(idxRemove, workingBenchActor);
		}
		);

}

void UCInventoryPanel_WorkingBench::RemoveAmountOfItem(UCItemBase* ItemToRemove, int32 AmountToRemove)
{
	int32 idxRemove = FindItemIndex(ItemToRemove);
	PerformActionIfHasAuthority(
		// Server
		[=](ACStructure_Placeable* workingBenchActor)
		{
			workingBenchActor->PerformRemoveAmountOfItem(idxRemove , AmountToRemove);
		},
		// Client
			[=](ACSurvivorController* playerController, ACStructure_Placeable* workingBenchActor)
		{
			playerController->RequestRemoveAmountOfItem(idxRemove, AmountToRemove,workingBenchActor);
		}
		
		);
}






int32 UCInventoryPanel_WorkingBench::FindItemIndex(UCItemBase * Item)
{
		int32 Index = WidgetItems.IndexOfByPredicate([Item](const UCItemBase* InItem)
			{
				return InItem == Item;
			});

		return Index;
}

void UCInventoryPanel_WorkingBench::AddItem(class UCItemBase* InItem, const int32 QuantityToAdd, class AActor* InActor)
{

	PerformActionIfHasAuthority(
		// Server
		[=](ACStructure_Placeable* workingBenchActor)
		{
			workingBenchActor->PerformAddItem(InItem->ID, QuantityToAdd, InItem->NumericData, InItem->ItemType);
		},
		// Client
			[=](ACSurvivorController* playerController, ACStructure_Placeable* workingBenchActor)
		{
			playerController->RequestAddItem(InItem->ID, QuantityToAdd, workingBenchActor, InItem->NumericData, InItem->ItemType);
		}
		);

}

void UCInventoryPanel_WorkingBench::SwapItem(UCItemBase* ItemOnBase, UCItemBase* ItemFromDrag)
{
	int32 idxBase = FindItemIndex(ItemOnBase);
	int32 idxDrag = FindItemIndex(ItemFromDrag);

	PerformActionIfHasAuthority(
		// Server
		[=](ACStructure_Placeable* workingBenchActor)
		{
			workingBenchActor->PerformSwapItem(idxBase, idxDrag);
		},
		// Client
			[=](ACSurvivorController* playerController, ACStructure_Placeable* workingBenchActor)
		{
			playerController->RequestSwapItem(idxBase, idxDrag, workingBenchActor);
		}
		);

}

bool UCInventoryPanel_WorkingBench::SplitExistingStack(UCItemBase* ItemIn, int32 AmountToSplit)
{
	if (ItemIn->Quantity - AmountToSplit <= 0) return false;
	//사용 가능 슬롯이 남아있으면
	int32 ItemIdx = FindItemIndex(ItemIn);

	return 	PerformActionIfHasAuthority(
		// Server
		[=](ACStructure_Placeable* workingBenchActor)
		{
			workingBenchActor->PerformSplitItem(ItemIdx, AmountToSplit);
		},
		// Client
			[=](ACSurvivorController* playerController, ACStructure_Placeable* workingBenchActor)
		{
			playerController->RequestSplitItem(ItemIdx, AmountToSplit, workingBenchActor);
		}
		);

}


//타입(오름) , 수량(내림), ID(오름) 순으로 정렬 
void UCInventoryPanel_WorkingBench::OnSortInventoryClicked()
{

	
	PerformActionIfHasAuthority(
		// Server
		[=](ACStructure_Placeable* workingBenchActor)
		{
			workingBenchActor->PerformSortInfoWidget();
		},
		// Client
			[=](ACSurvivorController* playerController, ACStructure_Placeable* workingBenchActor)
		{
			playerController->RequestSortInfoWidget(workingBenchActor);
		}
		);


}

//함수 매크로
bool UCInventoryPanel_WorkingBench::PerformActionIfHasAuthority(TFunction<void(ACStructure_Placeable*)> ServerAction, TFunction<void(ACSurvivorController*, ACStructure_Placeable*)> ClientAction)
{
	ACStructure_Placeable* workingBenchActor = Cast<ACStructure_Placeable>(OwnerActor);
	ACSurvivor* survivor = Cast<ACSurvivor>(this->GetOwningPlayerPawn());

	if (workingBenchActor && survivor)
	{
		if (survivor->HasAuthority())
		{
			ServerAction(workingBenchActor);
			return true;
		}
		else
		{
			ACSurvivorController* playerController = Cast<ACSurvivorController>(this->GetOwningPlayer());
			if (playerController)
			{
				ClientAction(playerController , workingBenchActor);
				return true;
			}
			else
			{
				CDebug::Print("playerController is not valid");
			}
		}
	}
	else
	{
		CDebug::Print("WorkingBenchActor or Survivor is not valid");
	}
	return false;
}