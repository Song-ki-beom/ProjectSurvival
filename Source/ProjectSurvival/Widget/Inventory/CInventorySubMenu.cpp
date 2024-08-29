// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/CInventorySubMenu.h"




void UCInventorySubMenu::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);
    if (InFocusEvent.GetCause() == EFocusCause::Mouse)
    {
        if (IsHovered())
        {
            SetKeyboardFocus();
            return;
        }
    }

	OnFocusOnSubMenuEnded.Broadcast();
}