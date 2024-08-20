// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/CInteractionWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interface/InteractionInterface.h"



void UCInteractionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InteractionProgressBar->PercentDelegate.BindUFunction(this,"UpdateInteractionProgress");

}

void UCInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	KeyPressText->SetText(FText::FromString("Press"));
	CurrentInteractionDuration = 0.0f;
}


float UCInteractionWidget::UpdateInteractionProgress()
{
	return 0.0f;
}




void UCInteractionWidget::UpdateWidget(const struct FInteractableData* InteractableData)
{
	switch (InteractableData->InteractableType)
	{
	case EInteractableType::Pickup: //Press 시 바로 픽업 가능하게 프로그레스 바x
		KeyPressText->SetText(FText::FromString("Press"));
		InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);

		if (InteractableData->Quantity == 1)
		{
			QuantityText->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			//NSLOCKTEXT <- 하나의 빌드에 국가 언어에 따른 Localize하여 번역된 문자열 제공  
			QuantityText->SetText(FText::Format(NSLOCTEXT("InteractionWidget","QuantityText","x{0}"),InteractableData->Quantity));
			QuantityText->SetVisibility(ESlateVisibility::Visible);

		}

		break;
	case EInteractableType::NonPlayerCharacter:
		break;
	case EInteractableType::Device:
		break;
	case EInteractableType::Toggle:
		break;
	case EInteractableType::Container:
		break;
	default:;
	}

	ActionText->SetText(InteractableData->Action);
	NameText->SetText(InteractableData->Name);
}



