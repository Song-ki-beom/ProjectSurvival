#include "Widget/CMainHUD.h"
#include "Widget/Menu/CInventoryMenu.h"
#include "Widget/Inventory/CInteractionWidget.h"
#include "Widget/Inventory/CInventorySubMenu.h"
#include "Widget/Inventory/CInventoryItemSlot.h"
#include "Widget/Inventory/CInventoryPanel_Placeable.h"
#include "Widget/Produce/CProduceWidget.h"
#include "Widget/Produce/CProduceWidget.h"
#include "Widget/Inventory/CEarnInfoPanel.h"
#include "Widget/Inventory/CItemBase.h"
#include "Widget/Chatting/CChattingBox.h"
#include "Widget/Inventory/CQuickSlot.h"
#include "Widget/Status/CLowHealthWidget.h"
#include "Widget/Status/CStatusPanel.h"
#include "Widget/Map/CMiniMap.h"
#include "Widget/Map/CWorldMap.h"
#include "Character/CSurvivorController.h"
#include "Utility/CDebug.h"

ACMainHUD::ACMainHUD()
{
}

void ACMainHUD::BeginPlay()
{
	Super::BeginPlay();

	if (InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UCInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1); //그보다 아래
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
		InteractionWidget->bIsFocusable = true;
	}

	//위젯들 등록 
	if (InventoryMenuClass)
	{
		//Widget 은 그래픽 요소를 지니고 있기 때문에 StaticClass() 가 아니라 에디터에 존재하는 요소를 참조로 가져와야 한다..InventoryMenuClass 가 그 예시
		SurvivorInventoryWidget = CreateWidget<UCInventoryMenu>(GetWorld(), InventoryMenuClass);
		SurvivorInventoryWidget->AddToViewport(5); //그려지는 zOrder 최상위 , 최우선으로 Interact 하기 위해 
		SurvivorInventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		SurvivorInventoryWidget->bIsFocusable = true;
	}
	
	//위젯들 등록 
	if (InventorySubMenuClass)
	{
		InventorySubMenuWidget = CreateWidget<UCInventorySubMenu>(GetWorld(), InventorySubMenuClass);
		InventorySubMenuWidget->OnFocusOnSubMenuEnded.AddUObject(this, &ACMainHUD::HideSubMenu);
	}

	if (ProduceWidgetClass)
	{
		SurvivorProduceWidget = CreateWidget<UCProduceWidget>(GetWorld(), ProduceWidgetClass);
		SurvivorProduceWidget->SetOwnerActor(nullptr, EWidgetCall::Survivor);
		SurvivorProduceWidget->AddToViewport(5);
		SurvivorProduceWidget->SetVisibility(ESlateVisibility::Collapsed);
		SurvivorProduceWidget->bIsFocusable = true;
	}

	if (EarnInfoPanelClass)
	{
		EarnInfoPanel = CreateWidget<UCEarnInfoPanel>(GetWorld(), EarnInfoPanelClass);
		EarnInfoPanel->AddToViewport(4);
		EarnInfoPanel->SetVisibility(ESlateVisibility::Visible);
		EarnInfoPanel->bIsFocusable = true;
	}

	if (QuickSlotClass)
	{
		QuickSlot = CreateWidget<UCQuickSlot>(GetWorld(), QuickSlotClass);

		FVector2D widgetSize = FVector2D(845, 80);
		FVector2D viewportSize;
		QuickSlot->SetDesiredSizeInViewport(widgetSize);
		GEngine->GameViewport->GetViewportSize(viewportSize);
		QuickSlot->SetAlignmentInViewport(FVector2D(0.5f, 1.0f));
		QuickSlot->SetPositionInViewport(FVector2D(viewportSize.X/2, viewportSize.Y - 1.0f));

		QuickSlot->AddToViewport(5);
		QuickSlot->SetVisibility(ESlateVisibility::Visible);
		QuickSlot->bIsFocusable = true;
	}

	if (StatusPanelClass)
	{
		StatusPanel = CreateWidget<UCStatusPanel>(GetWorld(), StatusPanelClass);
		StatusPanel->AddToViewport(1);
		StatusPanel->SetVisibility(ESlateVisibility::Visible);
		StatusPanel->bIsFocusable = false;

	}

	if (LowHealthWidgetClass)
	{
		LowHealthWidget = CreateWidget<UCLowHealthWidget>(GetWorld(), LowHealthWidgetClass);
		LowHealthWidget->AddToViewport(0);
		LowHealthWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ACMainHUD::SetWidgetVisibility(EWidgetCall InWidgetCall, class UUserWidget* InActorInventoryWidget, class UUserWidget* InActorProduceWidget, class AActor* InActor)
{
	//CDebug::Print("SetWidgetVisibility Called");

	switch (InWidgetCall)
	{
	case EWidgetCall::Survivor:
	{
		DisplaySurvivorInventoryWidget();
		DisplayProduceWidget(InWidgetCall);
		const FInputModeUIOnly InputMode;// 마우스와 키보드 입력이 UI에만 영향 
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
		break;
	}
	case EWidgetCall::Placeable:
	{
		DisplaySurvivorInventoryWidget();
		DisplayProduceWidget(InWidgetCall, InActorProduceWidget, InActor);
		DisplayActorInventory(InWidgetCall, InActorInventoryWidget, InActor);
		const FInputModeUIOnly InputMode;// 마우스와 키보드 입력이 UI에만 영향 
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
		break;
	}
	case EWidgetCall::CloseWidget:
	{
		//CDebug::Print("CloseWidget Called");
		if (SurvivorInventoryWidget)
		{
			SurvivorInventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (SurvivorProduceWidget)
		{
			SurvivorProduceWidget->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (PlaceableInventoryWidget)
		{
			PlaceableInventoryWidget->RemoveFromViewport();
			PlaceableInventoryWidget = nullptr;
		}

		if (PlaceableProduceWidget)
		{
			PlaceableProduceWidget->RemoveFromViewport();
			PlaceableProduceWidget = nullptr;
		}

		const FInputModeGameOnly InputMode;// 마우스와 키보드 입력이 InGame Action에만 반영
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
		break;
	}
	}
}

void ACMainHUD::DisplaySurvivorInventoryWidget()
{
	if (SurvivorInventoryWidget)
	{
		SurvivorInventoryWidget->SetVisibility(ESlateVisibility::Visible);
		SurvivorInventoryWidget->SetKeyboardFocus();
	}
	else
		CDebug::Print("SurvivorInventoryWidget is not valid");
}

void ACMainHUD::DisplayProduceWidget(EWidgetCall InWidgetCall, class UUserWidget* InActorProduceWidget, class AActor* InActor)
{
	switch (InWidgetCall)
	{
	case EWidgetCall::Survivor:
		if (SurvivorProduceWidget)
		{
			FVector2D widgetSize = FVector2D(580, 850);
			SurvivorProduceWidget->SetDesiredSizeInViewport(widgetSize);
			SurvivorProduceWidget->SetVisibility(ESlateVisibility::Visible);
			SurvivorProduceWidget->SetKeyboardFocus();
			SurvivorProduceWidget->RefreshProduceDetail();
		}
		break;
	case EWidgetCall::Placeable:
		if (InActorProduceWidget)
		{
			PlaceableProduceWidget = Cast<UCProduceWidget>(InActorProduceWidget);
			if (PlaceableProduceWidget)
			{
				FVector2D widgetSize = FVector2D(580, 850);
				PlaceableProduceWidget->SetDesiredSizeInViewport(widgetSize);
				PlaceableProduceWidget->SetVisibility(ESlateVisibility::Visible);
				PlaceableProduceWidget->bIsFocusable = true;
				PlaceableProduceWidget->AddToViewport(5);
				PlaceableProduceWidget->SetFocus();
			}
		}
		break;
	}


}

void ACMainHUD::DisplayActorInventory(EWidgetCall InWidgetCall, class UUserWidget* InWidget, class AActor* InActor)
{
	if (InWidget)
	{
		PlaceableInventoryWidget = Cast<UCInventoryPanel_Placeable>(InWidget);

		if (PlaceableInventoryWidget)
		{
			switch (InWidgetCall)
			{
			case EWidgetCall::Placeable:
			{
				FVector2D widgetSize = FVector2D(590, 440);
				FVector2D viewportSize;
				GEngine->GameViewport->GetViewportSize(viewportSize);
				PlaceableInventoryWidget->SetAlignmentInViewport(FVector2D(0.5f, 1.0f));
				PlaceableInventoryWidget->SetPositionInViewport(FVector2D(viewportSize.X / 2, viewportSize.Y / 2 - 15.0f));
				PlaceableInventoryWidget->SetDesiredSizeInViewport(widgetSize);
				PlaceableInventoryWidget->bIsFocusable = true;
				PlaceableInventoryWidget->SetVisibility(ESlateVisibility::Visible);
				PlaceableInventoryWidget->AddToViewport(5);
			}
			}
		}
	}
}

void ACMainHUD::ShowInteractionWidget()
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ACMainHUD::HideInteractionWidget()
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

//InteractionComponent에서 트레이스 후 새로운 상호작용 가능 액터를 검출할때마다 UI 갱신
void ACMainHUD::UpdateInteractionWidget(const FInteractableData* InteractableData)
{
	if (InteractionWidget)
	{
		if (InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			InteractionWidget->SetVisibility(ESlateVisibility::Visible);

		}
		InteractionWidget->UpdateWidget(InteractableData);
	}
}

void ACMainHUD::ShowSubMenu(FVector2D Position, class UCInventoryItemSlot* InSlotReference, ERightClickStartWidget InRightClickStartWidget, bool bIsStackable)
{
	if (InventorySubMenuWidget && !InventorySubMenuWidget->IsInViewport())
	{
		//CDebug::Print(FText::Format(FText::FromString("{0} , {1}"), Position.X, Position.Y).ToString());
		// 서브메뉴의 위치를 설정

		InventorySubMenuWidget->SetPositionInViewport(Position, true);
		// 서브메뉴 표시
		InventorySubMenuWidget->AddToViewport(6);
		InventorySubMenuWidget->bIsFocusable = true;
		InventorySubMenuWidget->SetKeyboardFocus();
		InventorySubMenuWidget->SetSlotReference(InSlotReference);
		InventorySubMenuWidget->UpdateSubMenu(InRightClickStartWidget, bIsStackable);
	}

}

void ACMainHUD::HideSubMenu()
{
	if (InventorySubMenuWidget &&InventorySubMenuWidget->IsInViewport())
	{
		// 서브메뉴를 뷰포트에서 제거
		InventorySubMenuWidget->RemoveFromViewport();
	}

}

void ACMainHUD::ToggleHiddenMenu()
{
	InteractionWidget->ToggleHiddenMenu();
}

void ACMainHUD::ShowHiddenMenu()
{
	InteractionWidget->ShowHiddenMenu();
}

void ACMainHUD::HideHiddenMenu()
{
	InteractionWidget->HideHiddenMenu();
}

void ACMainHUD::ExtraOptionButtonUp()
{
	InteractionWidget->MoveFocusToPrevButton();
}

void ACMainHUD::ExtraOptionButtonDown()
{
	InteractionWidget->MoveFocusToNextButton();
}

void ACMainHUD::AddEarnedInfo(UObject* EarnedItem)
{
	EarnInfoPanel->AddEarnedItemSlot(EarnedItem);
}