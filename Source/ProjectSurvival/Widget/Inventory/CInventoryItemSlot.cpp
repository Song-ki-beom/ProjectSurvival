// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/CInventoryItemSlot.h"
#include "Widget/Inventory/CInventoryTooltip.h"
#include "Widget/Inventory/CItemBase.h"
#include "Widget/Inventory/CDragItemVisual.h"
#include "Widget/Inventory/CItemDragDropOperation.h"
#include "Widget/Inventory/CInventoryPanel.h"
#include "Widget/Inventory/CInventoryPanel_WorkingBench.h"
#include "Widget/Inventory/CQuickSlot.h"
#include "Widget/Inventory/CInventorySubMenu.h"
#include "Widget/CMainHUD.h"
#include "ActorComponents/CInventoryComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Utility/CDebug.h"
#include "Kismet/GameplayStatics.h"

void UCInventoryItemSlot::NativeOnInitialized() //위젯 생성될때 호출
{
	Super::NativeOnInitialized();
	if (ToolTipClass)
	{
		ToolTip = CreateWidget<UCInventoryTooltip>(this, ToolTipClass);
		ToolTip->InventorySlotBeingHovered = this;
		ToggleTooltip();
		
	}

	if (this->GetTypedOuter<UUserWidget>() || this->GetParent()->GetTypedOuter<UUserWidget>())
	{
		if (this->GetTypedOuter<UUserWidget>())
		{
			OwnerWidget = this->GetTypedOuter<UUserWidget>();
		}
		else
		{
			OwnerWidget = this->GetParent()->GetTypedOuter<UUserWidget>();
		}
	}
}

void UCInventoryItemSlot::NativeConstruct() // 위젯 생성 -> UI 그래픽 요소 결정 후 호출 
{
	Super::NativeConstruct();

	if (ItemReference)
	{
		switch (ItemReference->ItemType)
		{
		case EItemType::Build:
			ItemBorder->SetBrushColor(FLinearColor::Gray);
			break;
		case EItemType::Harvest:
			ItemBorder->SetBrushColor(FLinearColor::Green);
				break;
		case EItemType::Hunt:
			ItemBorder->SetBrushColor(FLinearColor::Red);
			break;

		default:
			break;
		}


		ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);


		//아이템 중첩이 가능한 종류의 경우 
		if (ItemReference->NumericData.bIsStackable)
		{
			ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));
		}
		else
		{

			ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
		}


	}

}


//Drag 전에 위젯을 마우스오버 상태로 마우스 버튼을 클릭했는지 감지 
FReply UCInventoryItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) // 왼쪽 클릭 이벤트 처리
	{
		return  Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton); //드래그 감지 실행 , TakeWidget() <<- 이 메서드는 현재 위젯이 드래그 가능한 객체임을 반환함 
	}

	else if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton) // 오른쪽 클릭 이벤트 처리
	{
		// 생존자 인벤토리, 작업대 인벤토리가 아닐 경우 취소
		UCInventoryPanel* survivorInventoryPanel = Cast<UCInventoryPanel>(OwnerWidget);

		if (survivorInventoryPanel)
			RightClickStartWidget = ERightClickStartWidget::SurvivorInventory;

		if (!survivorInventoryPanel)
		{
			UCInventoryPanel_WorkingBench* workingBenchInventoryPanel = Cast<UCInventoryPanel_WorkingBench>(OwnerWidget);

			if (workingBenchInventoryPanel)
				RightClickStartWidget = ERightClickStartWidget::HideActionButtonWidget;
			else
			{
				RightClickStartWidget = ERightClickStartWidget::None;
				return Reply.Unhandled();
			}
		}

		if (RightClickStartWidget == ERightClickStartWidget::HideActionButtonWidget && !this->GetItemReference()->NumericData.bIsStackable)
			return Reply.Unhandled();

		ACMainHUD* mainHUD = Cast<ACMainHUD>(this->GetOwningPlayer()->GetHUD());
		if (mainHUD)
		{
			FVector2D mousePosition(0, 0);
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(mousePosition.X, mousePosition.Y);
			bool bIsStackable = this->GetItemReference()->NumericData.bIsStackable;
			if (!mainHUD->GetInventorySubMenu()->IsInViewport())
				ToggleTooltip();
			mainHUD->ShowSubMenu(FVector2D(mousePosition.X + 10, mousePosition.Y + 15), this, RightClickStartWidget, bIsStackable);
		}
		return Reply.Handled(); // 오른쪽 클릭 처리 완료
	}

	return Reply.Unhandled(); //다른 마우스 버튼일 경우 , 무응답 반환 


}

void UCInventoryItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{

}

//Tooltip Function 과 비슷하게 해당 위젯에 대한 마우스오버로 마우스가 해당 위젯을 Dragging 중인지 감지 
void UCInventoryItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	if (DragItemVisualClass)
	{


		UCDragItemVisual* DragVisual = CreateWidget<UCDragItemVisual>(this, DragItemVisualClass); //ui이므로 에디터에 있는 블루프린트 위젯 클래스로부터 동적 생성


		//Drag하여 움직일 때 표시되는 DragVisual 위젯에 현재 ItemSlot 의 UI 정보 복사 
		DragVisual->ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);
		
		DragVisual->ItemBorder->SetBrushColor(ItemBorder->BrushColor);

		if (ItemReference->NumericData.bIsStackable)
		{
			DragVisual->ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));

		}
		else
		{
			DragVisual->ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
		}

		UCItemDragDropOperation* DragItemOperation = NewObject<UCItemDragDropOperation>(); //데이터로만 이루어진 인스턴스이므로 NewObject 로 생성 
		DragItemOperation->SourceItem = ItemReference;
		DragItemOperation->SourceInventory = ItemReference->Inventory;

		DragItemOperation->DefaultDragVisual = DragVisual; //dragvisual 내장 변수, drag 시 보이는 default dragvisual 설정
		DragItemOperation->Pivot = EDragPivot::CenterCenter; //드래그 시 마우스의 어느 부분에 Attach 될 것인지 설정하는 offet Pivot 
		
		if (this->GetTypedOuter<UUserWidget>() || this->GetParent()->GetTypedOuter<UUserWidget>())
		{
			if (this->GetTypedOuter<UUserWidget>())
			{
				DragItemOperation->DragStartWidget = this->GetTypedOuter<UUserWidget>();
			}
			else
			{
				DragItemOperation->DragStartWidget = this->GetParent()->GetTypedOuter<UUserWidget>();
			}
		}
		else
			CDebug::Print("Start Widget is not valid ");


		//MainMenu에서 호출될 NativeOnDrop에서의 UDragDropOperation* OutOperation에 전달될 객체 설정 
		OutOperation = DragItemOperation;
	}
}


bool UCInventoryItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	//별도로 만든 DragDropOperation 생성 
	const UCItemDragDropOperation* ItemDragDrop = Cast<UCItemDragDropOperation>(InOperation);
	if (ItemDragDrop->SourceItem) //아이템이 DragDropOperation에서 감지되면 위젯 검사 (아이템이 Drag중이면 )
	{
		UCItemBase* DragItem  = ItemDragDrop->SourceItem;


	

		////UCInventoryPanel_WorkingBench을 가지고 있는지 검사 
		//bool IsOwnerWorkingBench = false;
		//UCInventoryPanel_WorkingBench* WorkingBenchPanel = Cast<UCInventoryPanel_WorkingBench>(ItemDragDrop->DragStartWidget);
		//if (WorkingBenchPanel)
		//{
		//	IsOwnerWorkingBench = true;
		//}

		if (DragItem == ItemReference)
		{
			CDebug::Print("Same Item Detected");
			return false;
		}
		else
		{
			UCInventoryPanel* dragStartInventoryPanel = Cast<UCInventoryPanel>(ItemDragDrop->DragStartWidget);
			UCInventoryPanel_WorkingBench* dragStartWorkingBenchPanel = Cast<UCInventoryPanel_WorkingBench>(ItemDragDrop->DragStartWidget);
			UCQuickSlot* dragStartQuickSlot = Cast<UCQuickSlot>(ItemDragDrop->DragStartWidget);

			UCInventoryPanel* ownerInventoryPanel = Cast<UCInventoryPanel>(OwnerWidget);
			UCInventoryPanel_WorkingBench* ownerWorkingBenchPanel = Cast<UCInventoryPanel_WorkingBench>(OwnerWidget);
			UCQuickSlot* ownerQuickSlot = Cast<UCQuickSlot>(OwnerWidget);

			// 드래그 시작 위젯과 오너 위젯의 타입을 확인 후 결과 결정
			if (dragStartInventoryPanel)
			{
				if (ownerInventoryPanel)
					DragDropResult = EDragDropResult::InventoryToInventory;
				else if (ownerWorkingBenchPanel)
					DragDropResult = EDragDropResult::InventoryToWorkingBench;
				else if (ownerQuickSlot)
					DragDropResult = EDragDropResult::InventoryToQuickSlot;
			}
			else if (dragStartWorkingBenchPanel)
			{
				if (ownerInventoryPanel)
					DragDropResult = EDragDropResult::WorkingBenchToInventory;
				else if (ownerWorkingBenchPanel)
					DragDropResult = EDragDropResult::WorkingBenchToWorkingBench;
				else if (ownerQuickSlot)
					DragDropResult = EDragDropResult::WorkingBenchToQuickSlot;
			}
			else if (dragStartQuickSlot)
			{
				if (ownerInventoryPanel)
					DragDropResult = EDragDropResult::QuickSlotToInventory;
				else if (ownerWorkingBenchPanel)
					DragDropResult = EDragDropResult::QuickSlotToWorkingBench;
				else if (ownerQuickSlot)
					DragDropResult = EDragDropResult::QuickSlotToQuickSlot;
			}

			FString EnumString = UEnum::GetValueAsString(DragDropResult);
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("DragDropResult: %s"), *EnumString));
		}
			


		//if (DragItem == ItemReference)
		//{
		//	CDebug::Print(TEXT("Same Item Detected"));
		//	return false;
		//}
		//else
		//{
			switch (DragDropResult)
			{
			case EDragDropResult::InventoryToInventory:
			{
				if ((DragItem != ItemReference) && (DragItem->ID == ItemReference->ID))
					ItemReference->Inventory->CombineItem(ItemReference, DragItem);
				else
					ItemReference->Inventory->SwapItem(ItemReference, DragItem);
				break;
			}
			case EDragDropResult::InventoryToWorkingBench:
				// 아무일X (workingbench자체 drop함수 실행되게)
				break;
			case EDragDropResult::InventoryToQuickSlot:
				// 장비일경우 스왑, 소비일경우 정보교환, 같은ID면 아무일X
				break;
			case EDragDropResult::WorkingBenchToWorkingBench:
			{
				UCInventoryPanel_WorkingBench* ownerWorkingBenchPanel = Cast<UCInventoryPanel_WorkingBench>(OwnerWidget);
				if ((DragItem != ItemReference) && (DragItem->ID == ItemReference->ID))
					ownerWorkingBenchPanel->CombineItem(ItemReference, DragItem);
				else
					ownerWorkingBenchPanel->SwapItem(ItemReference, DragItem);
				break;
			}
			case EDragDropResult::WorkingBenchToInventory:
				// 아무일X (inventory자체 drop함수 실행되게)
				break;
			case EDragDropResult::WorkingBenchToQuickSlot:
				// 아무일X - 퀵슬롯은 인벤토리에서만 등록 가능하게
				break;
			case EDragDropResult::QuickSlotToQuickSlot:
			{
				UCQuickSlot* ownerQuickSlot = Cast<UCQuickSlot>(OwnerWidget);
				ownerQuickSlot->SwapItemInfo(DragItem, ItemReference);
				break;
			}
			case EDragDropResult::QuickSlotToInventory:
				// 장비일경우 스왑, 소비일경우 정보교환, 같은ID면 아무일X
				break;
			case EDragDropResult::QuickSlotToWorkingBench:
				// 아무일X - workingbench 관련 이동은 인벤토리에서만 가능하게
				break;
			}

		//}

		/*else if ( (DragItem != ItemReference)&&(DragItem->ID  == ItemReference->ID))
		{
			if (IsOwnerWorkingBench)
			{
				return WorkingBenchPanel->CombineItem(ItemReference, DragItem);
			}
			else
			{
				return ItemReference->Inventory->CombineItem(ItemReference, DragItem);
			}
		}
		else
		{
			if (IsOwnerWorkingBench)
			{
				WorkingBenchPanel->SwapItem(ItemReference, DragItem);
			}
			else
			{
				ItemReference->Inventory->SwapItem(ItemReference, DragItem);

			}
			 return true;
		}*/
	}
	return false;
}

//서브메뉴의 On & off 에 따른 툴팁 토글 이벤트
void UCInventoryItemSlot::ToggleTooltip()
{

	if (bIsTooltipToggled == false)
	{
		SetToolTip(ToolTip);
		bIsTooltipToggled = true;
	}

	else
	{
		SetToolTip(nullptr);
		bIsTooltipToggled = false;

	}

	return;
}

bool UCInventoryItemSlot::Split(int32 InputNum)
{
	UCInventoryComponent* InventoryReference = ItemReference->Inventory;
	//bool answer = InventoryReference->SplitExistingStack(ItemReference , InputNum);

	//GetOwnerPanel 
	//해당 슬롯을 소유하는 UUserWidget 검사 
	
	//if (this->GetTypedOuter<UUserWidget>() || this->GetParent()->GetTypedOuter<UUserWidget>())
	//{
	//	if (this->GetTypedOuter<UUserWidget>())
	//	{
	//		OwnerWidget = this->GetTypedOuter<UUserWidget>();
	//	}
	//	else
	//	{
	//		OwnerWidget = this->GetParent()->GetTypedOuter<UUserWidget>();
	//	}
	//}
	if (OwnerWidget)
	{
		UCInventoryPanel_WorkingBench* WorkingBenchPanel = Cast<UCInventoryPanel_WorkingBench>(OwnerWidget);
		if (WorkingBenchPanel)
		{
			return WorkingBenchPanel->SplitExistingStack(ItemReference,InputNum);
		}
		else
		{
			return InventoryReference->SplitExistingStack(ItemReference, InputNum);;
		}
	}
		
	return false;
}
 
void UCInventoryItemSlot::SetItemQuantityText(int32 InQuantity)
{
	ItemQuantity->SetText(FText::AsNumber(InQuantity));
}


