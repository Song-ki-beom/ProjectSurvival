#include "Widget/Inventory/CInventoryPanel_Placeable.h"
#include "Widget/Inventory/CInventoryPanel.h"
#include "Widget/Inventory/CItemDragDropOperation.h"
#include "Widget/Inventory/CItemBase.h"
#include "Widget/Inventory/CInventoryItemSlot.h"
#include "Widget/Produce/CProduceWidget.h"
#include "Widget/Build/CBuildWidget.h"
#include "Widget/CMainHUD.h"
#include "ActorComponents/CActorInventoryComponent.h"
#include "Build/CStructure_Placeable.h"
#include "Components/WrapBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "CGameStateBase.h"
#include "Utility/CDebug.h"
#include "Character/CSurvivorController.h"

bool UCInventoryPanel_Placeable::Initialize()
{
	bool Sucess = Super::Initialize();

	if (IsValid(SortItemButton))
		SortItemButton->OnClicked.AddDynamic(this, &UCInventoryPanel_Placeable::OnSortInventoryClicked);

	WidgetItems.Reserve(100);

	if (!Sucess)
		return false;

	return true;
}

FReply UCInventoryPanel_Placeable::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
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
		if (OwnerActor && OwnerActor->GetPlaceableProduceWidget())
			OwnerActor->GetPlaceableProduceWidget()->StartProduce();

		return FReply::Handled();
	}

	return FReply::Unhandled();
}

bool UCInventoryPanel_Placeable::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
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

				ACSurvivorController* survivorController = Cast<ACSurvivorController>(this->GetOwningPlayer());
				if (survivorController)
				{
					if (survivorController->GetBuildWidget())
						survivorController->GetBuildWidget()->RefreshBuildWidgetQuantity(ItemDragDrop->SourceItem->ID);
					else
						CDebug::Print("survivorController->GetBuildWidget() is not Valid", FColor::Red);
				}
				else
					CDebug::Print("survivorController is not Valid", FColor::Red);

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

void UCInventoryPanel_Placeable::SetInventoryWindowName(FText InText)
{
	InventoryWindowName->SetText(InText);
}

void UCInventoryPanel_Placeable::RefreshPlaceableInventory()
{
	//WrapBox에 정보를 추가하기 전에 기존 이미지 삭제
	PlaceableInventoryPanel->ClearChildren();

	for (UCItemBase* tempItem : WidgetItems)
	{
		if (tempItem)
		{
			//CDebug::Print("tempItem is Valid", FColor::Magenta);
			UCInventoryItemSlot* ItemSlot = CreateWidget<UCInventoryItemSlot>(this, InventorySlotClass);
			if (ItemSlot)
			{
				ItemSlot->SetItemReference(tempItem);
				PlaceableInventoryPanel->AddChildToWrapBox(ItemSlot);
			}
			else
				CDebug::Print("ItemSlot is not Valid");
		}
		else
		{
			CDebug::Print("tempItem is not Valid", FColor::Magenta);
		}
	}
}


bool UCInventoryPanel_Placeable::CombineItem(UCItemBase* ItemOnBase, UCItemBase* ItemFromDrag)
{
		//if (ItemOnBase->IsFullItemStack() || ItemFromDrag->IsFullItemStack()) //둘중에 하나가 풀스택이면 Swap Item 
		//{
		//	SwapItem(ItemOnBase, ItemFromDrag);
		//	return true;
		//}
	
		//int32 idxBase = FindItemIndex(ItemOnBase);
		//int32 idxDrag = FindItemIndex(ItemFromDrag);

		//
		//return PerformActionIfHasAuthority(
		//	// Server
		//	[=](ACStructure_Placeable* placeableActor)
		//	{
		//		placeableActor->PerformCombineItem(idxBase, idxDrag);
		//	},
		//	// Client
		//		[=](ACSurvivorController* playerController, ACStructure_Placeable* placeableActor)
		//	{
		//		playerController->RequestCombineItem(idxBase, idxDrag, placeableActor);
		//	}
		//	);

	if (ItemOnBase->IsFullItemStack() || ItemFromDrag->IsFullItemStack()) //둘중에 하나가 풀스택이면 Swap Item 
	{
		SwapItem(ItemOnBase, ItemFromDrag);
		return true;
	}

	int32 idxBase = FindItemIndex(ItemOnBase);
	int32 idxDrag = FindItemIndex(ItemFromDrag);


	return PerformActionIfHasAuthority(
		// Server
		[=](ACStructure_Placeable* placeableActor)
		{
			placeableActor->BroadcastCombineItem(idxBase, idxDrag);
		},
		// Client
			[=](ACSurvivorController* playerController, ACStructure_Placeable* placeableActor)
		{
			playerController->RequestCombineItem(idxBase, idxDrag, placeableActor);
		}
		);

	
}


void UCInventoryPanel_Placeable::SetWidgetItems(TArray<UCItemBase*> InArray)
{
	if (WidgetItems.Num() == 0)
	{
		WidgetItems = InArray;
		return;
	}

	
	if (WidgetItems.Num() != InArray.Num())
	{
		WidgetItems.SetNum(InArray.Num());  // WidgetItems의 크기를 InArray와 동일하게 설정
	}

	for (int32 i = 0; i < InArray.Num(); ++i)
	{
		if (WidgetItems[i] == nullptr)
		{
			WidgetItems[i] = InArray[i];
			continue;
		}
		WidgetItems[i]->ID = InArray[i]->ID;
		WidgetItems[i]->Quantity = InArray[i]->Quantity;
		WidgetItems[i]->ItemType = InArray[i]->ItemType;
		WidgetItems[i]->TextData = InArray[i]->TextData;
		WidgetItems[i]->ItemStats = InArray[i]->ItemStats;
		WidgetItems[i]->NumericData = InArray[i]->NumericData;
		WidgetItems[i]->AssetData = InArray[i]->AssetData;
		WidgetItems[i]->ProduceData = InArray[i]->ProduceData;
		WidgetItems[i]->BuildData = InArray[i]->BuildData;
		WidgetItems[i]->HuntData = InArray[i]->HuntData;
		WidgetItems[i]->bIsCopy = InArray[i]->bIsCopy;
		
	}

}

void UCInventoryPanel_Placeable::RemoveItem(UCItemBase* ItemToRemove)
{
	//int32 idxRemove = FindItemIndex(ItemToRemove);
	//PerformActionIfHasAuthority(
	//	// Server
	//	[=](ACStructure_Placeable* placeableActor)
	//	{
	//		placeableActor->PerformRemoveItem(idxRemove);
	//	},
	//	// Client
	//		[=](ACSurvivorController* playerController, ACStructure_Placeable* placeableActor)
	//	{
	//		playerController->RequestRemoveItem(idxRemove, placeableActor);
	//	}
	//	);

		int32 idxRemove = FindItemIndex(ItemToRemove);
	PerformActionIfHasAuthority(
		// Server
		[=](ACStructure_Placeable* placeableActor)
		{
			placeableActor->BroadcastRemoveItem(idxRemove);
		},
		// Client
			[=](ACSurvivorController* playerController, ACStructure_Placeable* placeableActor)
		{
			playerController->RequestRemoveItem(idxRemove, placeableActor);
		}
		);
	

}

void UCInventoryPanel_Placeable::RemoveAmountOfItem(UCItemBase* ItemToRemove, int32 AmountToRemove)
{
	//int32 idxRemove = FindItemIndex(ItemToRemove);
	//PerformActionIfHasAuthority(
	//	// Server
	//	[=](ACStructure_Placeable* placeableActor)
	//	{
	//		placeableActor->PerformRemoveAmountOfItem(idxRemove , AmountToRemove);
	//	},
	//	// Client
	//		[=](ACSurvivorController* playerController, ACStructure_Placeable* placeableActor)
	//	{
	//		playerController->RequestRemoveAmountOfItem(idxRemove, AmountToRemove, placeableActor);
	//	}
	//	
	//	);

	int32 idxRemove = FindItemIndex(ItemToRemove);
	PerformActionIfHasAuthority(
		// Server
		[=](ACStructure_Placeable* placeableActor)
		{
			placeableActor->BroadcastRemoveAmountOfItem(idxRemove, AmountToRemove);
		},
		// Client
			[=](ACSurvivorController* playerController, ACStructure_Placeable* placeableActor)
		{
			playerController->RequestRemoveAmountOfItem(idxRemove, AmountToRemove, placeableActor);
		}

		);
}






int32 UCInventoryPanel_Placeable::FindItemIndex(UCItemBase * Item)
{
	if (Item == nullptr) return -1;
		int32 Index = WidgetItems.IndexOfByPredicate([Item](const UCItemBase* InItem)
			{
				return InItem == Item;
			});

		return Index;
}

void UCInventoryPanel_Placeable::AddItem(class UCItemBase* InItem, const int32 QuantityToAdd, class AActor* InActor)
{

	//PerformActionIfHasAuthority(
	//	// Server
	//	[=](ACStructure_Placeable* placeableActor)
	//	{
	//		placeableActor->PerformAddItem(InItem->ID, QuantityToAdd, InItem->NumericData, InItem->ItemType, InItem->ItemStats);
	//	},
	//	// Client
	//		[=](ACSurvivorController* playerController, ACStructure_Placeable* placeableActor)
	//	{
	//		playerController->RequestAddItem(InItem->ID, QuantityToAdd, placeableActor, InItem->NumericData, InItem->ItemType, InItem->ItemStats);
	//	}
	//	);

	PerformActionIfHasAuthority(
		// Server
		[=](ACStructure_Placeable* placeableActor)
		{
			placeableActor->BroadcastAddItem(InItem->ID, QuantityToAdd, InItem->NumericData, InItem->ItemType, InItem->ItemStats, InItem->HuntData.WeaponType);
		},
		// Client
			[=](ACSurvivorController* playerController, ACStructure_Placeable* placeableActor)
		{
			playerController->RequestAddItem(InItem->ID, QuantityToAdd, placeableActor, InItem->NumericData, InItem->ItemType, InItem->ItemStats, InItem->HuntData.WeaponType);
		}
		);

}

void UCInventoryPanel_Placeable::RepairItem(UCItemBase* ItemToRepair)
{
	int32 idxRepair = FindItemIndex(ItemToRepair);

	PerformActionIfHasAuthority(
		// Server
		[=](ACStructure_Placeable* placeableActor)
		{
			placeableActor->BroadcastRepair(idxRepair, ItemToRepair->ID, 1, ItemToRepair->NumericData, ItemToRepair->ItemType, ItemToRepair->ItemStats, ItemToRepair->HuntData.WeaponType);
		},
		// Client
			[=](ACSurvivorController* playerController, ACStructure_Placeable* placeableActor)
		{
			playerController->RequestRepair(idxRepair, placeableActor, ItemToRepair->ID, 1, ItemToRepair->NumericData, ItemToRepair->ItemType, ItemToRepair->ItemStats, ItemToRepair->HuntData.WeaponType);
		}
		);
}

void UCInventoryPanel_Placeable::SwapItem(UCItemBase* ItemOnBase, UCItemBase* ItemFromDrag)
{
	//int32 idxBase = FindItemIndex(ItemOnBase);
	//int32 idxDrag = FindItemIndex(ItemFromDrag);

	//if (idxBase < 0 || idxDrag<0) return; //Item이 업데이트 되어 사라졌으면 

	//PerformActionIfHasAuthority(
	//	// Server
	//	[=](ACStructure_Placeable* placeableActor)
	//	{
	//		placeableActor->PerformSwapItem(idxBase, idxDrag);
	//	},
	//	// Client
	//		[=](ACSurvivorController* playerController, ACStructure_Placeable* placeableActor)
	//	{
	//		playerController->RequestSwapItem(idxBase, idxDrag, placeableActor);
	//	}
	//	);

	int32 idxBase = FindItemIndex(ItemOnBase);
	int32 idxDrag = FindItemIndex(ItemFromDrag);

	if (idxBase < 0 || idxDrag<0) return; //Item이 업데이트 되어 사라졌으면 

	PerformActionIfHasAuthority(
		// Server
		[=](ACStructure_Placeable* placeableActor)
		{
			placeableActor->BroadcastSwapItem(idxBase, idxDrag);
		},
		// Client
			[=](ACSurvivorController* playerController, ACStructure_Placeable* placeableActor)
		{
			playerController->RequestSwapItem(idxBase, idxDrag, placeableActor);
		}
		);
	

}

bool UCInventoryPanel_Placeable::SplitExistingStack(UCItemBase* ItemIn, int32 AmountToSplit)
{
	//if (ItemIn->Quantity - AmountToSplit <= 0) return false;
	////사용 가능 슬롯이 남아있으면
	//int32 ItemIdx = FindItemIndex(ItemIn);
	//if (ItemIdx < 0) return false; //Item이 업데이트 되어 사라졌으면 

	//return 	PerformActionIfHasAuthority(
	//	// Server
	//	[=](ACStructure_Placeable* placeableActor)
	//	{
	//		placeableActor->PerformSplitItem(ItemIdx, AmountToSplit);
	//	},
	//	// Client
	//		[=](ACSurvivorController* playerController, ACStructure_Placeable* placeableActor)
	//	{
	//		playerController->RequestSplitItem(ItemIdx, AmountToSplit, placeableActor);
	//	}
	//	);

	if (ItemIn->Quantity - AmountToSplit <= 0 || AmountToSplit <= 0) return false;
	//사용 가능 슬롯이 남아있으면
	int32 ItemIdx = FindItemIndex(ItemIn);
	if (ItemIdx < 0) return false; //Item이 업데이트 되어 사라졌으면 

	return 	PerformActionIfHasAuthority(
		// Server
		[=](ACStructure_Placeable* placeableActor)
		{
			placeableActor->BroadcastSplitItem(ItemIdx, AmountToSplit);
		},
		// Client
			[=](ACSurvivorController* playerController, ACStructure_Placeable* placeableActor)
		{
			playerController->RequestSplitItem(ItemIdx, AmountToSplit, placeableActor);
		}
		);
}


//타입(오름) , 수량(내림), ID(오름) 순으로 정렬 
void UCInventoryPanel_Placeable::OnSortInventoryClicked()
{

	
	//PerformActionIfHasAuthority(
	//	// Server
	//	[=](ACStructure_Placeable* placeableActor)
	//	{
	//		placeableActor->PerformSortInfoWidget();
	//	},
	//	// Client
	//		[=](ACSurvivorController* playerController, ACStructure_Placeable* placeableActor)
	//	{
	//		playerController->RequestSortInfoWidget(placeableActor);
	//	}
	//	);

	PerformActionIfHasAuthority(
		// Server
		[=](ACStructure_Placeable* placeableActor)
		{
			placeableActor->BroadcastSortInfoWidget();
		},
		// Client
			[=](ACSurvivorController* playerController, ACStructure_Placeable* placeableActor)
		{
			playerController->RequestSortInfoWidget(placeableActor);
		}
		);


}

//함수 매크로
bool UCInventoryPanel_Placeable::PerformActionIfHasAuthority(TFunction<void(ACStructure_Placeable*)> ServerAction, TFunction<void(ACSurvivorController*, ACStructure_Placeable*)> ClientAction)
{
	ACStructure_Placeable* placeableActor = Cast<ACStructure_Placeable>(OwnerActor);
	ACSurvivor* survivor = Cast<ACSurvivor>(this->GetOwningPlayerPawn());

	if (placeableActor && survivor)
	{
		if (survivor->HasAuthority())
		{
			ServerAction(placeableActor);
			return true;
		}
		else
		{
			ACSurvivorController* playerController = Cast<ACSurvivorController>(this->GetOwningPlayer());
			if (playerController)
			{
				ClientAction(playerController , placeableActor);
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
		CDebug::Print("placeableActor or Survivor is not valid");
	}
	return false;
}