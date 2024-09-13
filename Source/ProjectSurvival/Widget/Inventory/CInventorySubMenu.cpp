// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/CInventorySubMenu.h"
#include "Widget/Inventory/CInventoryItemSlot.h"
#include "Components/EditableText.h" 
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Utility/CDebug.h"


void UCInventorySubMenu::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);
    if (InFocusEvent.GetCause() == EFocusCause::Mouse)
    {
        if (!IsHovered())
        {
            OnFocusOnSubMenuEnded.Broadcast();
            SlotReference->ToggleTooltip();

           // NativeOnMouseEnter(Geometry, PointerEvent);
            return;
        }
    }

	
}

void UCInventorySubMenu::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{


}

void UCInventorySubMenu::NativeConstruct()
{
  

}

bool UCInventorySubMenu::Initialize()
{
    bool Success = Super::Initialize();
   
    if (IsValid(SplitButton))
        SplitButton->OnClicked.AddDynamic(this, &UCInventorySubMenu::HandleOnSplitButtonClicked);
    if (IsValid(FinalSplitButton))
        FinalSplitButton->OnClicked.AddDynamic(this, &UCInventorySubMenu::HandleOnFinalSplitButtonClicked);
    if (IsValid(CancelButton))
        CancelButton->OnClicked.AddDynamic(this, &UCInventorySubMenu::HandleOnCancelButtonClicked);
    
    TextItemType = EItemType::NoType;

    SubMenuSizeBox->SetVisibility(ESlateVisibility::Visible);
    SplitMenuSizeBox->SetVisibility(ESlateVisibility::Collapsed);




    if (!Success) return false;
    return true;
}







void UCInventorySubMenu::UpdateSubMenu()
{
    SubMenuSizeBox->SetVisibility(ESlateVisibility::Visible);
    SplitMenuSizeBox->SetVisibility(ESlateVisibility::Collapsed);

    if (SlotReference && TextItemType != SlotReference->GetItemReference()->ItemType)
    {
        ActionButton->OnClicked.Clear();
        TextItemType = SlotReference->GetItemReference()->ItemType;

        switch (TextItemType)
        {
        case EItemType::Harvest:
            ActionButton->SetVisibility(ESlateVisibility::Collapsed);
            break;
        case EItemType::Hunt:
            if (IsValid(ActionButton))
            {
                ActionButton->SetVisibility(ESlateVisibility::Visible);
                ActionButton->OnClicked.AddDynamic(this, &UCInventorySubMenu::HandleOnUseButtonClicked);
            }
            ActionText->SetText(SlotReference->GetItemReference()->TextData.UsageText);
            break;

        case EItemType::Consumable:
            if (IsValid(ActionButton)) 
            {
                ActionButton->SetVisibility(ESlateVisibility::Visible);
                ActionButton->OnClicked.AddDynamic(this, &UCInventorySubMenu::HandleOnUseButtonClicked);
            }
            ActionText->SetText(SlotReference->GetItemReference()->TextData.UsageText);
            break;
        case EItemType::Build:
            if (IsValid(ActionButton)) 
            {
                ActionButton->SetVisibility(ESlateVisibility::Visible);
                ActionButton->OnClicked.AddDynamic(this, &UCInventorySubMenu::HandleOnBuildButtonClicked);

            }
            ActionText->SetText(SlotReference->GetItemReference()->TextData.UsageText);
            break;
        default:
            break;
        }

    }

}



void UCInventorySubMenu::HandleOnUseButtonClicked()
{
   // OnUseButtonClicked.Broadcast();
    OnFocusOnSubMenuEnded.Broadcast();
    SlotReference->ToggleTooltip();
    //Item Use 구현 부분 
}


void UCInventorySubMenu::HandleOnBuildButtonClicked()
{
    
    OnFocusOnSubMenuEnded.Broadcast();
    SlotReference->ToggleTooltip();
    //Build 등록 구현 부분 
}


void UCInventorySubMenu::HandleOnSplitButtonClicked()
{
    
    //Split 구현 부분 

    SubMenuSizeBox->SetVisibility(ESlateVisibility::Collapsed);
    SplitMenuSizeBox->SetVisibility(ESlateVisibility::Visible);



}

void UCInventorySubMenu::HandleOnFinalSplitButtonClicked()
{

    FText InputNumText = SplitQuantityEditText->GetText();
    if (!InputNumText.IsEmpty())
    {

        // FText 를 FString으로 변환
        FString InputNumString = InputNumText.ToString();
        // IsNumeric -> 숫자인지 확인하는 bool Return 함수 
        bool IsNumeric = InputNumString.IsNumeric();

        if (IsNumeric)
        {
            // 숫자로 변환
            int32 InputNum = FCString::Atoi(*InputNumString);
            if (SlotReference->Split(InputNum))
            {
                OnFocusOnSubMenuEnded.Broadcast();
                SlotReference->ToggleTooltip();
            }
            else
            {
                CDebug::Print("The Split failed Due to Quantity or Slot issue");
            }
        }
        else
        {
            CDebug::Print("The Split Input Text is not composed with only Number");
        }
    }


    //슬롯에서 스플릿 구현 
}

void UCInventorySubMenu::HandleOnCancelButtonClicked()
{

    OnFocusOnSubMenuEnded.Broadcast();
     SlotReference->ToggleTooltip();
    

}


