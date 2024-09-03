// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/CInteractionWidget.h"
#include "Widget/Inventory/CItemBase.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Utility/CDebug.h"
#include "Interface/InteractionInterface.h"



UCInteractionWidget::UCInteractionWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	ConstructorHelpers::FObjectFinder<UDataTable> DataTableAsset(TEXT("DataTable'/Game/PirateIsland/Include/Datas/Widget/Inventory/DT_Items.DT_Items'"));
	if (DataTableAsset.Succeeded())
	{
		ItemDataTable = DataTableAsset.Object;
	}

}



void UCInteractionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InteractionProgressBar->PercentDelegate.BindUFunction(this,"UpdateInteractionProgress");
	MoreInfoBox->SetVisibility(ESlateVisibility::Collapsed);
	ExtraOptionBox->SetVisibility(ESlateVisibility::Collapsed);
}

void UCInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CurrentInteractionDuration = 0.0f;

	ExtraButtonArray.Add(CancelButton);
	CancelButton->OnClicked.AddDynamic(this, &UCInteractionWidget::OnCancelButtonClicked);

	ExtraButtonArray.Add(RecallButton);
	RecallButton->OnClicked.AddDynamic(this, &UCInteractionWidget::OnRecallButtonClicked);

	ExtraButtonArray.Add(DestroyButton);
	DestroyButton->OnClicked.AddDynamic(this, &UCInteractionWidget::OnDestroyButtonClicked);

	ExtraButtonArray.Add(RepairButton);
	RepairButton->OnClicked.AddDynamic(this, &UCInteractionWidget::OnRepairButtonClicked);

	ExtraButtonFoucsIndex = 0;
	DefaultButtonStyle = CancelButton->WidgetStyle;

	
		
	


}

FReply UCInteractionWidget::NativeOnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	CDebug::Print(TEXT("NativeOnMouseWheel Dectected"));
	//if (MouseEvent.GetWheelDelta() < 0) // 마우스 휠 아래로
	//{
	//	MoveFocusToNextButton();
	//}
	//else if (MouseEvent.GetWheelDelta() > 0) // 마우스 휠 위로
	//{
	//	MoveFocusToPreviousButton();
	//}

	return FReply::Handled(); 
}

FReply UCInteractionWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	CDebug::Print(TEXT("NativeOnMouseButtonDown Dectected"));
	/*SetFocus();*/
	return Super::NativeOnMouseButtonDown(InGeometry,InMouseEvent);
}

void UCInteractionWidget::MoveFocusToNextButton()
{
	if (ExtraOptionBox->GetVisibility() == ESlateVisibility::Visible)
	{
		int32 prevIndex = ExtraButtonFoucsIndex;
		ExtraButtonArray[prevIndex]->SetStyle(DefaultButtonStyle);

		ExtraButtonFoucsIndex = (ExtraButtonFoucsIndex + 1) % 4; //인덱스 증가
		
		
		int32 NewIndex = ExtraButtonFoucsIndex;
		FButtonStyle HoveredStyle = ExtraButtonArray[NewIndex]->WidgetStyle;
		// Hovered 상태의 스타일을 현재 상태로 설정합니다.
		HoveredStyle.Normal = HoveredStyle.Hovered;

		ExtraButtonArray[NewIndex]->SetStyle(HoveredStyle);

	}
}

void UCInteractionWidget::MoveFocusToPrevButton()
{
	if (ExtraOptionBox->GetVisibility() == ESlateVisibility::Visible)
	{
		int32 prevIndex = ExtraButtonFoucsIndex;
		ExtraButtonArray[prevIndex]->SetStyle(DefaultButtonStyle);


		ExtraButtonFoucsIndex -= 1;
		if (ExtraButtonFoucsIndex < 0) ExtraButtonFoucsIndex = ExtraButtonArray.Num()-1;


		int32 NewIndex = ExtraButtonFoucsIndex;
		FButtonStyle HoveredStyle = ExtraButtonArray[NewIndex]->WidgetStyle;
		// Hovered 상태의 스타일을 현재 상태로 설정합니다.
		HoveredStyle.Normal = HoveredStyle.Hovered;

		ExtraButtonArray[NewIndex]->SetStyle(HoveredStyle);
	}
}

void UCInteractionWidget::OnCancelButtonClicked()
{
	return;
}

void UCInteractionWidget::OnRecallButtonClicked()
{
	return;
}

void UCInteractionWidget::OnDestroyButtonClicked()
{
	return;

}

void UCInteractionWidget::OnRepairButtonClicked()
{
	return;

}






void UCInteractionWidget::ShowHiddenMenu()
{
	
	if (InteractType == EInteractableType::Build)
	{
		if (ExtraOptionBox->GetVisibility() == ESlateVisibility::Collapsed)
		{
		ExtraOptionBox->SetVisibility(ESlateVisibility::Visible);
		ExtraButtonFoucsIndex = 0;
		FButtonStyle HoveredStyle = ExtraButtonArray[ExtraButtonFoucsIndex]->WidgetStyle;
		HoveredStyle.Normal = HoveredStyle.Hovered;
		ExtraButtonArray[ExtraButtonFoucsIndex]->SetStyle(HoveredStyle);
		}

	}

	else 
	{
		if (MoreInfoBox->GetVisibility() == ESlateVisibility::Collapsed)
		{
			MoreInfoBox->SetVisibility(ESlateVisibility::Visible);
		}

	}



}

void UCInteractionWidget::HideHiddenMenu()
{
	if (InteractType == EInteractableType::Build)
	{
		if (ExtraOptionBox->GetVisibility() == ESlateVisibility::Visible)
		{
		ExtraOptionBox->SetVisibility(ESlateVisibility::Collapsed);
		ExtraButtonArray[ExtraButtonFoucsIndex]->SetStyle(DefaultButtonStyle);
		ExtraButtonArray[ExtraButtonFoucsIndex]->OnClicked.Broadcast();
		ExtraButtonFoucsIndex = 0;

		}
	}
	else
	{
		if (MoreInfoBox->GetVisibility() == ESlateVisibility::Visible)
		{
			MoreInfoBox->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UCInteractionWidget::ToggleHiddenMenu()
{

	if (InteractType == EInteractableType::Build)
	{
		if (ExtraOptionBox->GetVisibility() == ESlateVisibility::Collapsed)
		{
		ExtraOptionBox->SetVisibility(ESlateVisibility::Visible);
		ExtraButtonFoucsIndex = 0;
		FButtonStyle HoveredStyle = ExtraButtonArray[ExtraButtonFoucsIndex]->WidgetStyle;
		HoveredStyle.Normal = HoveredStyle.Hovered;
		ExtraButtonArray[ExtraButtonFoucsIndex]->SetStyle(HoveredStyle);
		}
		else if (ExtraOptionBox->GetVisibility() == ESlateVisibility::Visible)
		{
		ExtraButtonArray[ExtraButtonFoucsIndex]->SetStyle(DefaultButtonStyle);
		ExtraButtonArray[ExtraButtonFoucsIndex]->OnClicked.Broadcast();
		ExtraOptionBox->SetVisibility(ESlateVisibility::Collapsed);
		ExtraButtonFoucsIndex = 0;
		}

	}

	else
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

	
}




float UCInteractionWidget::UpdateInteractionProgress()
{
	return 0.0f;
}




void UCInteractionWidget::UpdateWidget(const struct FInteractableData* InteractableData)
{

	NameText->SetText(InteractableData->Name); // 아이템 이름 
	ActionText->SetText(InteractableData->Action); //상호작용 Text
	InteractType = InteractableData->InteractableType;
	switch (InteractType)
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
		ActionText->SetText(FText::FromString(TEXT("기타 옵션"))); //상호작용 Text

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



	//MoreInfo 업데이트 부분 
	if (ItemDataTable && InteractableData->InteractableType != EInteractableType::Build) //Empty String 인지 체크 
	{

		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(InteractableData->ID, InteractableData->ID.ToString());
		if (ItemData)
		{

			switch (ItemData->ItemType)
			{
			case EItemType::Build:
				ItemType->SetText(FText::FromString("Build"));
				DamageValue->SetVisibility(ESlateVisibility::Collapsed);

				break;

			case EItemType::Harvest:
				ItemType->SetText(FText::FromString("Harvest"));
				DamageValue->SetVisibility(ESlateVisibility::Collapsed);
				break;


			case EItemType::Hunt:
				ItemType->SetText(FText::FromString("Hunting Tool"));
				break;

			case EItemType::Consumable:
				ItemType->SetText(FText::FromString("Consumable"));
				DamageValue->SetVisibility(ESlateVisibility::Collapsed);
				break;


			}
			ItemName->SetText(ItemData->TextData.Name);
			DamageValue->SetText(FText::AsNumber(ItemData->ItemStats.DamageValue));
			ItemDescription->SetText(ItemData->TextData.Description);

			const FString WeightInfo = TEXT("무게: ") + FString::SanitizeFloat(InteractableData->Quantity * ItemData->NumericData.Weight); //총합 Weight 
			StackWeight->SetText(FText::FromString(WeightInfo));


			const FString StackInfo = TEXT("최대 허용 수량: ") + FString::FromInt(ItemData->NumericData.MaxStackSize);
			MaxStackSize->SetText(FText::FromString(StackInfo));

		}

	}
}



