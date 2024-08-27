// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CMainHUD.h"
#include "Widget/Menu/CMainMenu.h"
#include "Widget/Inventory/CInteractionWidget.h"

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
	if (MainMenuClass)
	{
		//Widget 은 그래픽 요소를 지니고 있기 때문에 StaticClass() 가 아니라 에디터에 존재하는 요소를 참조로 가져와야 한다..MainMenuClass 가 그 예시
		MainMenuWidget = CreateWidget<UCMainMenu>(GetWorld(), MainMenuClass);
		MainMenuWidget->AddToViewport(5); //그려지는 zOrder 최상위 , 최우선으로 Interact 하기 위해 
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	





}


void ACMainHUD::DisplayMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = true;
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ACMainHUD::HideMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = false;
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
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
		const FInputModeGameAndUI InputMode;// 마우스와 키보드 입력이 InGame Action에만 반영
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
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

