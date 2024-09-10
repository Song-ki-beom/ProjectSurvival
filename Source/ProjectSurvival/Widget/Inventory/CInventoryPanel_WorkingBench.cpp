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
#include "GameFramework/PlayerState.h"

bool UCInventoryPanel_WorkingBench::Initialize()
{
	bool Sucess = Super::Initialize();

	if (!Sucess)
		return false;

	OnWorkingBenchUpdated.AddUObject(this, &UCInventoryPanel_WorkingBench::RefreshWorkingBenchInventory);

	OnAdditionalItem.BindUObject(this, &UCInventoryPanel_WorkingBench::AddAdditionalItem);


	
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
					APlayerController* playerController = Cast<APlayerController>(GetOwningPlayer());
					if (playerController && playerController->PlayerState)
					{
						int32 playerIndex = playerController->PlayerState->GetPlayerId(); // PlayerState에서 고유 ID를 가져옴
						CDebug::Print(TEXT("드랍한 플레이어 인덱스 : "), playerIndex);
						AddItem(ItemDragDrop->SourceItem, ItemDragDrop->SourceItem->Quantity, OwnerActor, playerIndex);
						
					}
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


	//ACSurvivorController* survivorController = Cast<ACSurvivorController>(GetWorld()->GetFirstPlayerController());
	//if (IsValid(survivorController))
	//{
	//	survivorController->RefreshWorkingBenchInventory(InIndex);
	//}
	//else
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("lobbySurvivorController is not valid - ACLobbySurvivor"));
	//}

}

void UCInventoryPanel_WorkingBench::AddItem(class UCItemBase* InItem, const int32 QuantityToAdd, class AActor* InActor, int32 InPlayerIndex)
{
	//UCItemBase* NewItem;

	//NewItem = InItem->CreateItemCopy();



	ACStructure_Placeable* workingBenchActor = Cast<ACStructure_Placeable>(InActor);
	if (workingBenchActor)
	{
		////////

		ACSurvivor* survivor = Cast<ACSurvivor>(this->GetOwningPlayerPawn());
		if (survivor->HasAuthority())
		{
			workingBenchActor->PerformAddID(InItem->ID, QuantityToAdd, InItem->NumericData, InPlayerIndex);
		}
		else
		{
			ACSurvivorController* playerController = Cast<ACSurvivorController>(this->GetOwningPlayer());
			if (playerController)
			{
				playerController->RequestAddItem(InItem->ID, QuantityToAdd, workingBenchActor, InItem->NumericData, InPlayerIndex);
			}
			else
				CDebug::Print("playerController is not valid");
		}
	}
	else
		CDebug::Print("Widget Owner is not Valid");
}

void UCInventoryPanel_WorkingBench::AddAdditionalItem(FName InID, int32 InQuantity, FItemNumericData InNumericData, int32 InPlayerIndex)
{
	//ACStructure_Placeable* workingBenchActor = Cast<ACStructure_Placeable>(InActor);
	//if (workingBenchActor)
	{
		ACSurvivor* survivor = Cast<ACSurvivor>(this->GetOwningPlayerPawn());
		if (survivor->HasAuthority())
		{
			OwnerActor->PerformAddID(InID, InQuantity, InNumericData, InPlayerIndex);
		}
		else
		{
			APlayerController* playerController = Cast<APlayerController>(GetOwningPlayer());
			if (playerController && playerController->PlayerState)
			{
				int32 playerIndex = playerController->PlayerState->GetPlayerId();

				if (playerIndex == InPlayerIndex)
				{
					CDebug::Print(TEXT("같음!!!!"), FColor::Green);
					ACSurvivorController* survivorController = Cast<ACSurvivorController>(playerController);
					if (survivorController)
						survivorController->RequestAddItem(InID, InQuantity, OwnerActor, InNumericData, InPlayerIndex);
					else
						CDebug::Print("playerController is not valid");
				}
				else
					CDebug::Print(TEXT("다름!!!!"), FColor::Red);
			}
			
		}
	}
	//else
	//	CDebug::Print("Widget Owner is not Valid");
}

