// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/CInteractionWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Interface/InteractionInterface.h"



void UCInteractionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InteractionProgressBar->PercentDelegate.BindUFunction(this,"UpdateInteractionProgress");
	MoreInfoBox->SetVisibility(ESlateVisibility::Collapsed);

}

void UCInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CurrentInteractionDuration = 0.0f;



	

}




void UCInteractionWidget::ShowMoreInfo()
{
	if (MoreInfoBox->GetVisibility() == ESlateVisibility::Collapsed)
	{
		MoreInfoBox->SetVisibility(ESlateVisibility::Visible);
	}

}

void UCInteractionWidget::HideMoreInfo()
{
	if (MoreInfoBox->GetVisibility() == ESlateVisibility::Visible)
	{
		MoreInfoBox->SetVisibility(ESlateVisibility::Collapsed);
	}

}

void UCInteractionWidget::ToggleMoreInfo()
{
	if (MoreInfoBox->GetVisibility() == ESlateVisibility::Collapsed)
	{
		MoreInfoBox->SetVisibility(ESlateVisibility::Visible);
	}
	else if (MoreInfoBox->GetVisibility() == ESlateVisibility::Visible)
	{
		MoreInfoBox->SetVisibility(ESlateVisibility::Collapsed);
	}
}





float UCInteractionWidget::UpdateInteractionProgress()
{
	return 0.0f;
}




void UCInteractionWidget::UpdateWidget(const struct FInteractableData* InteractableData)
{

	NameText->SetText(InteractableData->Name); // 아이템 이름 
	ActionText->SetText(InteractableData->Action); //상호작용 Text

	switch (InteractableData->InteractableType)
	{
	case EInteractableType::Pickup: //Press 시 바로 픽업 가능하게 프로그레스 바x
		InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EInteractableType::NonPlayerCharacter:
		InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EInteractableType::Device: 
		InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EInteractableType::Container:
		InteractionProgressBar->SetVisibility(ESlateVisibility::Visible);
		break;
	case EInteractableType::Build:
		InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);
	default:;
	}

	if (InteractableData->Quantity == 1)
	{
		QuantityText->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		//NSLOCKTEXT <- 하나의 빌드에 국가 언어에 따른 Localize하여 번역된 문자열 제공  
		QuantityText->SetText(FText::Format(NSLOCTEXT("InteractionWidget", "QuantityText", "x{0}"), InteractableData->Quantity));
		QuantityText->SetVisibility(ESlateVisibility::Visible);

	}
	

}



