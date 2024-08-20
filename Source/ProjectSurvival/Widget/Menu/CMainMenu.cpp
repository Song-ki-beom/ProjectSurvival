// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Menu/CMainMenu.h"
#include "Character/CSurvivor.h"

void UCMainMenu::NativeOnInitialized() 
{

	Super::NativeOnInitialized();
}

void UCMainMenu::NativeConstruct()
{

	Super::NativeConstruct();

	PlayerCharacter = Cast<ACSurvivor>(GetOwningPlayerPawn());

}

bool UCMainMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return 	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	//cast operation to item drag drop, ensure player is valid , call drop item on player
}