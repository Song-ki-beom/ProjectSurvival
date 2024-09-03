// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/CInventorySubMenu.h"
#include "Widget/Inventory/CInventoryItemSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"



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

    TextItemType = EItemType::NoType;

    if (!Success) return false;
    return true;
}







void UCInventorySubMenu::UpdateSubMenu()
{
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
    OnFocusOnSubMenuEnded.Broadcast();
    SlotReference->ToggleTooltip();
    //Split 구현 부분 
}