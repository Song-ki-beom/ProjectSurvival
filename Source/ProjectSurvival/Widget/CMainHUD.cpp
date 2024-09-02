// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CMainHUD.h"
#include "Widget/Menu/CInventoryMenu.h"
#include "Widget/Inventory/CInteractionWidget.h"
#include "Widget/Inventory/CInventorySubMenu.h"
#include "Widget/Inventory/CInventoryItemSlot.h"
#include "Widget/Produce/CProduceWidget.h"
#include "Widget/Produce/CProduceWidget.h"
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
	}

	//위젯들 등록 
	if (InventoryMenuClass)
	{
		//Widget 은 그래픽 요소를 지니고 있기 때문에 StaticClass() 가 아니라 에디터에 존재하는 요소를 참조로 가져와야 한다..InventoryMenuClass 가 그 예시
		InventoryMenuWidget = CreateWidget<UCInventoryMenu>(GetWorld(), InventoryMenuClass);
		InventoryMenuWidget->AddToViewport(5); //그려지는 zOrder 최상위 , 최우선으로 Interact 하기 위해 
		InventoryMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
		InventoryMenuWidget->OnMainMenuToggled.AddUObject(this, &ACMainHUD::ToggleMenu);
		InventoryMenuWidget->bIsFocusable = true;

	}
	
	//위젯들 등록 
	if (InventorySubMenuClass)
	{
		InventorySubMenuWidget = CreateWidget<UCInventorySubMenu>(GetWorld(), InventorySubMenuClass);
		InventorySubMenuWidget->OnFocusOnSubMenuEnded.AddUObject(this, &ACMainHUD::HideSubMenu);
	}

	if (ProduceWidgetClass)
	{
		ProduceWidget = CreateWidget<UCProduceWidget>(GetWorld(), ProduceWidgetClass);
		ProduceWidget->AddToViewport(5);
		ProduceWidget->SetVisibility(ESlateVisibility::Collapsed);
		ProduceWidget->OnProduceWidgetToggled.AddUObject(this, &ACMainHUD::ToggleMenu);
	}



}




void ACMainHUD::HideMenu()
{
	if (InventoryMenuWidget)
	{
		bIsMenuVisible = false;
		InventoryMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (ProduceWidget)
	{
		ProduceWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ACMainHUD::ToggleMenu()
{
	if (bIsMenuVisible)
	{
		HideMenu();

		const FInputModeGameOnly InputMode;// 마우스와 키보드 입력이 InGame Action에만 반영
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);



	}
	else
	{
		DisplayMenu();
		const FInputModeUIOnly InputMode;// 마우스와 키보드 입력이 UI에만 영향 
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);

	}


}

void ACMainHUD::DisplayMenu()
{
	if (InventoryMenuWidget)
	{
		bIsMenuVisible = true;
		InventoryMenuWidget->SetVisibility(ESlateVisibility::Visible);
		InventoryMenuWidget->SetKeyboardFocus();
	}

	if (ProduceWidget)
	{
		ProduceWidget->SetDesiredSizeInViewport(FVector2D(580, 850));
		ProduceWidget->SetPositionInViewport(FVector2D(50, 50));
		ProduceWidget->SetVisibility(ESlateVisibility::Visible);
		ProduceWidget->bIsFocusable = true;
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


void ACMainHUD::ShowSubMenu(FVector2D Position , UCInventoryItemSlot* InSlotReference)
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
		InventorySubMenuWidget->UpdateSubMenu();
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

void ACMainHUD::ToggleMoreInfo()
{
	InteractionWidget->ToggleMoreInfo();
}




void ACMainHUD::ShowMoreInfo()
{
	InteractionWidget->ShowMoreInfo();
}

void ACMainHUD::HideMoreInfo()
{
	InteractionWidget->HideMoreInfo();

}
