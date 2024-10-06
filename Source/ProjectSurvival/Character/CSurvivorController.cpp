// Fill out your copyright notice in the Description page of Project Settings.


#include "CSurvivorController.h"
#include "CSurvivor.h"
#include "Engine/Engine.h"
#include "InputCoreTypes.h"
#include "GameFramework/Actor.h"
#include "ActorComponents/CBuildComponent.h"
#include "ActorComponents/CMovingComponent.h"
#include "Widget/CMainHUD.h"
#include "Widget/Build/CBuildWidget.h"
#include "Widget/Build/CBuildItemSlot.h"
#include "Widget/Produce/CProduceWidget.h"
#include "Widget/Inventory/CInventoryPanel_Placeable.h"
#include "Utility/CDebug.h"
#include "CGameInstance.h"
#include "Build/CStructure_Placeable.h"
#include "Build/CStructure_Door.h"
#include "Widget/CMainHUD.h"
#include "Widget/Chatting/CChattingBox.h"
#include "Widget/Inventory/CQuickSlot.h"
#include "Widget/Map/CMiniMap.h"
#include "Widget/Map/CWorldMap.h"
#include "Widget/Inventory/CInventoryItemSlot.h"
#include "Components/SizeBox.h"

ACSurvivorController::ACSurvivorController()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> buildWidgetFinder(TEXT("WidgetBlueprint'/Game/PirateIsland/Include/Blueprints/Widget/Build/WBP_BuildWidget.WBP_BuildWidget_C'"));
	if (buildWidgetFinder.Succeeded())
	{
		BuildWidgetClass = buildWidgetFinder.Class;
		//CDebug::Print("buildWidgetFinder Succeeded", FColor::Green);
	}
	else
		CDebug::Print("buildWidgetFinder Failed", FColor::Red);

	//static ConstructorHelpers::FClassFinder<UUserWidget> registerRespawnLocationFinder(TEXT("WidgetBlueprint'/Game/PirateIsland/Include/Blueprints/Widget/Build/WBP_BuildWidget.WBP_BuildWidget_C'"));
	//if (registerRespawnLocationFinder.Succeeded())
	//{
	//	RespawnLocationRegisterClass = registerRespawnLocationFinder.Class;
	//}
	//else
	//	CDebug::Print("registerRespawnLocationFinder Failed", FColor::Red);

	bIsBuildWidgetOn = false;
	bIsProduceWidgetOn = false;
}

void ACSurvivorController::BeginPlay()
{
	Super::BeginPlay();
	this->SetInputMode(FInputModeGameOnly());
	GetSurvivor();
	SetupInputFunction();
	GetWorld()->GetTimerManager().SetTimer(UpdateListenerTransformHandle, this, &ACSurvivorController::UpdateListenerTransform, 0.1f, true);
}

void ACSurvivorController::GetSurvivor()
{
	Survivor = Cast<ACSurvivor>(this->GetCharacter());
}

void ACSurvivorController::SetupBuildWidget()
{
	if (!IsValid(BuildWidget))
	{
		BuildWidget = CreateWidget<UCBuildWidget>(this, BuildWidgetClass);
		BuildWidget->AddToViewport();
		BuildWidget->SetVisibility(ESlateVisibility::Collapsed);
		bIsBuildWidgetOn = false;
	}
}

void ACSurvivorController::SetupInputFunction()
{
	if (IsValid(Survivor))
	{
		InputComponent->BindKey(EKeys::T, IE_Pressed, this, &ACSurvivorController::HoldAxe);
		InputComponent->BindKey(EKeys::LeftMouseButton, IE_Pressed, this, &ACSurvivorController::DoAction);
		InputComponent->BindKey(EKeys::RightMouseButton, IE_Pressed, this, &ACSurvivorController::SubAction);
		InputComponent->BindAction("Build", IE_Pressed, this, &ACSurvivorController::ToggleBuildWidget);
		InputComponent->BindAction("SelectQ", IE_Pressed, this, &ACSurvivorController::SelectQ);
		InputComponent->BindAction("SelectW", IE_Pressed, this, &ACSurvivorController::SelectW);
		InputComponent->BindAction("SelectE", IE_Pressed, this, &ACSurvivorController::SelectE);
		InputComponent->BindAction("SelectE", IE_Released, this, &ACSurvivorController::ReleaseE);
		InputComponent->BindAction("SelectA", IE_Pressed, this, &ACSurvivorController::SelectA);
		InputComponent->BindAction("SelectS", IE_Pressed, this, &ACSurvivorController::SelectS);
		InputComponent->BindAction("SelectD", IE_Pressed, this, &ACSurvivorController::SelectD);
		InputComponent->BindAction("SelectZ", IE_Pressed, this, &ACSurvivorController::SelectZ);
		InputComponent->BindAction("SelectX", IE_Pressed, this, &ACSurvivorController::SelectX);
		InputComponent->BindAction("SelectC", IE_Pressed, this, &ACSurvivorController::SelectC);
		InputComponent->BindAction("Inventory", IE_Pressed, this, &ACSurvivorController::ShowWidget);
		InputComponent->BindAction("MouseWheelUp", IE_Pressed, this,&ACSurvivorController::HandleMouseWheelUp);
		InputComponent->BindAction("MouseWheelDown", IE_Pressed,this, &ACSurvivorController::HandleMouseWheelDown);
		InputComponent->BindAction("Chat", IE_Pressed,this, &ACSurvivorController::FocusChattingBox);
		InputComponent->BindAction("WorldMap", IE_Pressed,this, &ACSurvivorController::ToggleWorldMap);
		InputComponent->BindAction("QuickSlot1", IE_Pressed, this, &ACSurvivorController::PressQuickSlot1);
		InputComponent->BindAction("QuickSlot2", IE_Pressed, this, &ACSurvivorController::PressQuickSlot2);
		InputComponent->BindAction("QuickSlot3", IE_Pressed, this, &ACSurvivorController::PressQuickSlot3);
		InputComponent->BindAction("QuickSlot4", IE_Pressed, this, &ACSurvivorController::PressQuickSlot4);
		InputComponent->BindAction("QuickSlot5", IE_Pressed, this, &ACSurvivorController::PressQuickSlot5);
		InputComponent->BindAction("QuickSlot6", IE_Pressed, this, &ACSurvivorController::PressQuickSlot6);
		InputComponent->BindAction("QuickSlot7", IE_Pressed, this, &ACSurvivorController::PressQuickSlot7);
		InputComponent->BindAction("QuickSlot8", IE_Pressed, this, &ACSurvivorController::PressQuickSlot8);
		InputComponent->BindAction("QuickSlot9", IE_Pressed, this, &ACSurvivorController::PressQuickSlot9);
		InputComponent->BindAction("QuickSlot10", IE_Pressed, this, &ACSurvivorController::PressQuickSlot10);

		UCMovingComponent* movingComponent = Survivor->GetMovingComponent();
		if (IsValid(movingComponent))
		{
			InputComponent->BindAxis("MoveForward", movingComponent, &UCMovingComponent::OnMoveForward);
			InputComponent->BindAxis("MoveRight", movingComponent, &UCMovingComponent::OnMoveRight);
			InputComponent->BindAxis("VerticalLook", movingComponent, &UCMovingComponent::OnVerticalLook);
			InputComponent->BindAxis("HorizontalLook", movingComponent, &UCMovingComponent::OnHorizontalLook);
		}
	}
	else
	{
		//CDebug::Print("CSurvivor is not valid");
	}
}

void ACSurvivorController::UpdateListenerTransform()
{
	APawn* controlledPawn = this->GetPawn();
	if (controlledPawn)
	{
		FVector listenerLocation = controlledPawn->GetActorLocation();
		FRotator listenerRotation = controlledPawn->GetActorRotation();
		SetAudioListenerOverride(nullptr, listenerLocation, listenerRotation);
	}
}

void ACSurvivorController::ToggleBuildWidget()
{
	if (bIsProduceWidgetOn)
		return;

	if (IsValid(BuildWidget))
	{
		if (bIsBuildWidgetOn)
		{
			bIsBuildWidgetOn = false;
			BuildWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			bIsBuildWidgetOn = true;
			BuildWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void ACSurvivorController::SelectQ()
{
	if (bIsBuildWidgetOn)
	{
		if (IsValid(BuildWidget->GetStructureClass(ESelectedStructure::Q)))
		{
			if (BuildWidget->GetBuildItemSlotQ()->GetBuildItemSlotQuantity() == 0)
				return;

			TSubclassOf<ACStructure> structureClass = BuildWidget->GetStructureClass(ESelectedStructure::Q);
			EBuildStructureElement structureElement = BuildWidget->GetStructureElement(ESelectedStructure::Q);
			FName structureID = BuildWidget->GetBuildItemSlotQ()->GetBuildItemSlotID();
			Survivor->SelectStructure(ESelectedStructure::Q, structureClass, structureElement, structureID);
			ToggleBuildWidget();
		}
	}
}

void ACSurvivorController::SelectW()
{
	if (bIsBuildWidgetOn)
	{
		if (IsValid(BuildWidget->GetStructureClass(ESelectedStructure::W)))
		{
			if (BuildWidget->GetBuildItemSlotW()->GetBuildItemSlotQuantity() == 0)
				return;

			TSubclassOf<ACStructure> structureClass = BuildWidget->GetStructureClass(ESelectedStructure::W);
			EBuildStructureElement structureElement = BuildWidget->GetStructureElement(ESelectedStructure::W);
			FName structureID = BuildWidget->GetBuildItemSlotW()->GetBuildItemSlotID();
			Survivor->SelectStructure(ESelectedStructure::W, structureClass, structureElement, structureID);
			ToggleBuildWidget();
		}
	}
}

void ACSurvivorController::SelectE()
{
	if (bIsBuildWidgetOn)
	{
		if (IsValid(BuildWidget->GetStructureClass(ESelectedStructure::E)))
		{
			if (BuildWidget->GetBuildItemSlotE()->GetBuildItemSlotQuantity() == 0)
				return;

			TSubclassOf<ACStructure> structureClass = BuildWidget->GetStructureClass(ESelectedStructure::E);
			EBuildStructureElement structureElement = BuildWidget->GetStructureElement(ESelectedStructure::E);
			FName structureID = BuildWidget->GetBuildItemSlotE()->GetBuildItemSlotID();
			Survivor->SelectStructure(ESelectedStructure::E, structureClass, structureElement, structureID);
			ToggleBuildWidget();
		}
	}
	else 
	{

		Survivor->StartInteract();
	}

}

void ACSurvivorController::ReleaseE()
{
	Survivor->FinishInteract();

}

void ACSurvivorController::SelectA()
{
	if (bIsBuildWidgetOn)
	{
		if (IsValid(BuildWidget->GetStructureClass(ESelectedStructure::A)))
		{
			if (BuildWidget->GetBuildItemSlotA()->GetBuildItemSlotQuantity() == 0)
				return;

			TSubclassOf<ACStructure> structureClass = BuildWidget->GetStructureClass(ESelectedStructure::A);
			EBuildStructureElement structureElement = BuildWidget->GetStructureElement(ESelectedStructure::A);
			FName structureID = BuildWidget->GetBuildItemSlotA()->GetBuildItemSlotID();
			Survivor->SelectStructure(ESelectedStructure::A, structureClass, structureElement, structureID);
			ToggleBuildWidget();
		}
	}
}

void ACSurvivorController::SelectS()
{
	if (bIsBuildWidgetOn)
	{
		if (IsValid(BuildWidget->GetStructureClass(ESelectedStructure::S)))
		{
			if (BuildWidget->GetBuildItemSlotS()->GetBuildItemSlotQuantity() == 0)
				return;

			TSubclassOf<ACStructure> structureClass = BuildWidget->GetStructureClass(ESelectedStructure::S);
			EBuildStructureElement structureElement = BuildWidget->GetStructureElement(ESelectedStructure::S);
			FName structureID = BuildWidget->GetBuildItemSlotS()->GetBuildItemSlotID();
			Survivor->SelectStructure(ESelectedStructure::S, structureClass, structureElement, structureID);
			ToggleBuildWidget();
		}
	}
}

void ACSurvivorController::SelectD()
{
	if (bIsBuildWidgetOn)
	{
		if (IsValid(BuildWidget->GetStructureClass(ESelectedStructure::D)))
		{
			if (BuildWidget->GetBuildItemSlotD()->GetBuildItemSlotQuantity() == 0)
				return;

			TSubclassOf<ACStructure> structureClass = BuildWidget->GetStructureClass(ESelectedStructure::D);
			EBuildStructureElement structureElement = BuildWidget->GetStructureElement(ESelectedStructure::D);
			FName structureID = BuildWidget->GetBuildItemSlotD()->GetBuildItemSlotID();
			Survivor->SelectStructure(ESelectedStructure::D, structureClass, structureElement, structureID);
			ToggleBuildWidget();
		}
	}
}

void ACSurvivorController::SelectZ()
{
	if (bIsBuildWidgetOn)
	{
		if (IsValid(BuildWidget->GetStructureClass(ESelectedStructure::Z)))
		{
			if (BuildWidget->GetBuildItemSlotZ()->GetBuildItemSlotQuantity() == 0)
				return;

			TSubclassOf<ACStructure> structureClass = BuildWidget->GetStructureClass(ESelectedStructure::Z);
			EBuildStructureElement structureElement = BuildWidget->GetStructureElement(ESelectedStructure::Z);
			FName structureID = BuildWidget->GetBuildItemSlotZ()->GetBuildItemSlotID();
			Survivor->SelectStructure(ESelectedStructure::Z, structureClass, structureElement, structureID);
			ToggleBuildWidget();
		}
	}
}

void ACSurvivorController::SelectX()
{
	if (bIsBuildWidgetOn)
	{
		if (IsValid(BuildWidget->GetStructureClass(ESelectedStructure::X)))
		{
			if (BuildWidget->GetBuildItemSlotX()->GetBuildItemSlotQuantity() == 0)
				return;

			TSubclassOf<ACStructure> structureClass = BuildWidget->GetStructureClass(ESelectedStructure::X);
			EBuildStructureElement structureElement = BuildWidget->GetStructureElement(ESelectedStructure::X);
			FName structureID = BuildWidget->GetBuildItemSlotX()->GetBuildItemSlotID();
			Survivor->SelectStructure(ESelectedStructure::X, structureClass, structureElement, structureID);
			ToggleBuildWidget();
		}
	}
}

void ACSurvivorController::SelectC()
{
	if (bIsBuildWidgetOn)
	{
		if (IsValid(BuildWidget->GetStructureClass(ESelectedStructure::C)))
		{
			if (BuildWidget->GetBuildItemSlotC()->GetBuildItemSlotQuantity() == 0)
				return;

			TSubclassOf<ACStructure> structureClass = BuildWidget->GetStructureClass(ESelectedStructure::C);
			EBuildStructureElement structureElement = BuildWidget->GetStructureElement(ESelectedStructure::C);
			FName structureID = BuildWidget->GetBuildItemSlotC()->GetBuildItemSlotID();
			Survivor->SelectStructure(ESelectedStructure::C, structureClass, structureElement, structureID);
			ToggleBuildWidget();
		}
	}
}

void ACSurvivorController::DoAction()
{
	if (Survivor)
	{
		if (Survivor->GetBuildComponent()->CheckIsBuilding())
		{
			Survivor->Build();
			return;
		}
		Survivor->DoAction();
	}
}

void ACSurvivorController::SubAction()
{
	if (Survivor)
	{
		if (Survivor->GetBuildComponent()->CheckIsBuilding())
		{
			Survivor->CancleBuild();
			return;
		}
		Survivor->SubAction();
	}
}

void ACSurvivorController::HoldAxe()
{
	if (Survivor)
	{
		Survivor->HoldAxe();
	}
}

void ACSurvivorController::ShowWidget()
{
	if (bIsBuildWidgetOn)
		return;

	ACMainHUD* mainHUD = Cast<ACMainHUD>(this->GetHUD());
	if (mainHUD)
		mainHUD->SetWidgetVisibility(EWidgetCall::Survivor);
	//Survivor->ToggleMenu();
}

void ACSurvivorController::HandleMouseWheelUp()
{
	Survivor->HandleMouseWheelUp();
}

void ACSurvivorController::HandleMouseWheelDown()
{
	Survivor->HandleMouseWheelDown();
}

void ACSurvivorController::FocusChattingBox()
{
	UCGameInstance* gameInstance = Cast<UCGameInstance>(GetGameInstance());
	if (gameInstance)
	{
		if (gameInstance->ChattingBox)
		{
			gameInstance->ChattingBox->SetInputMode();
		}
		else
			CDebug::Print("gameInstance->ChattingBox is not Valid");
	}
	else
		CDebug::Print("gameInstance is not Valid");
}

void ACSurvivorController::ToggleWorldMap()
{
	if (bIsProduceWidgetOn)
		return;

	UCGameInstance* gameInstance = Cast<UCGameInstance>(GetWorld()->GetGameInstance());
	if (gameInstance)
	{
		if (gameInstance->WorldMap && gameInstance->MiniMap)
		{
			if (gameInstance->WorldMap->GetVisibility() == ESlateVisibility::Hidden)
				gameInstance->WorldMap->SetVisibility(ESlateVisibility::Visible);
			else
				gameInstance->WorldMap->SetVisibility(ESlateVisibility::Hidden);
		
			if (gameInstance->MiniMap->GetVisibility() == ESlateVisibility::Hidden)
				gameInstance->MiniMap->SetVisibility(ESlateVisibility::Visible);
			else
				gameInstance->MiniMap->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ACSurvivorController::PressQuickSlot1()
{
}

void ACSurvivorController::PressQuickSlot2()
{
}

void ACSurvivorController::PressQuickSlot3()
{
}

void ACSurvivorController::PressQuickSlot4()
{
}

void ACSurvivorController::PressQuickSlot5()
{
	ACMainHUD* mainHUD = Cast<ACMainHUD>(this->GetHUD());
	if (!mainHUD)
		return;

	UCQuickSlot* quickSlot = mainHUD->GetQuickSlotWidget();
	if (!quickSlot)
		return;

	USizeBox* sizeBox = quickSlot->GetSizeBoxArray()[4];
	if (!sizeBox)
		return;

	UCInventoryItemSlot* quickSlotItemWidget = Cast<UCInventoryItemSlot>(sizeBox->GetChildAt(0));
	if (!quickSlotItemWidget)
		return;

	UCItemBase* quickSlotItem = quickSlotItemWidget->GetItemReference();
	if (!quickSlotItem)
		return;

	if (quickSlotItem->ItemType == EItemType::Hunt)
	{
		int32 durability = quickSlotItem->ItemStats.RemainDurability--;
		quickSlotItemWidget->SetRemainDurability(durability);
	}

	// ID나 여러타입에 따른 스위치문함수 필요
}

void ACSurvivorController::PressQuickSlot6()
{
}

void ACSurvivorController::PressQuickSlot7()
{
}

void ACSurvivorController::PressQuickSlot8()
{
}

void ACSurvivorController::PressQuickSlot9()
{
}

void ACSurvivorController::PressQuickSlot10()
{
}

void ACSurvivorController::ShowBuildInteractWidget()
{
	//RespawnLocationRegisterClass
}

void ACSurvivorController::RequestAddItem_Implementation(FName ItemID, int32 InQuantity, class ACStructure_Placeable* InPlaceable, FItemNumericData InNumericData, EItemType ItemType, FItemStats InItemStats)
{
	//InPlaceable->PerformAddItem(ItemID, InQuantity, InNumericData, ItemType, InItemStats);
	InPlaceable->BroadcastAddItem(ItemID, InQuantity, InNumericData, ItemType, InItemStats);
}

void ACSurvivorController::RequestSwapItem_Implementation(int32 idxBase, int32  idxDrag, class ACStructure_Placeable* InPlaceable)
{
	//InPlaceable->PerformSwapItem(idxBase, idxDrag);
	InPlaceable->BroadcastSwapItem(idxBase, idxDrag);
};

void ACSurvivorController::RequestSortInfoWidget_Implementation(class ACStructure_Placeable* InPlaceable)
{
	//InPlaceable->PerformSortInfoWidget();
	InPlaceable->BroadcastSortInfoWidget();
}

void ACSurvivorController::RequestSplitItem_Implementation(int32 ItemIdx, int32 AmountToSplit, class ACStructure_Placeable* InPlaceable)
{
	//InPlaceable->PerformSplitItem(ItemIdx, AmountToSplit);
	InPlaceable->BroadcastSplitItem(ItemIdx, AmountToSplit);
}

void ACSurvivorController::RequestCombineItem_Implementation(int32 idxBase, int32  idxDrag, class ACStructure_Placeable* InPlaceable)
{
	//InPlaceable->PerformCombineItem(idxBase, idxDrag);
	InPlaceable->BroadcastCombineItem(idxBase, idxDrag);
}

void ACSurvivorController::RequestRemoveItem_Implementation(int32 idxRemove, class ACStructure_Placeable* InPlaceable)
{
	//InPlaceable->PerformRemoveItem(idxRemove);
	InPlaceable->BroadcastRemoveItem(idxRemove);
}

void ACSurvivorController::RequestRemoveAmountOfItem_Implementation(int32 idxRemove, int32 AmountToRemove, class ACStructure_Placeable* InPlaceable)
{
	//InPlaceable->PerformRemoveAmountOfItem(idxRemove, AmountToRemove);
	InPlaceable->BroadcastRemoveAmountOfItem(idxRemove, AmountToRemove);
}

void ACSurvivorController::RequestAddProduceItemToQueue_Implementation(FName ItemID, class ACStructure_Placeable* InPlaceable)
{
	InPlaceable->BroadcastAddProduceItemToQueue(ItemID, InPlaceable);
}

void ACSurvivorController::RequestIgnite_Implementation(class ACStructure_Placeable* InPlaceable)
{
	InPlaceable->BroadcastIgnite();
}

void ACSurvivorController::RequestExtinguish_Implementation(class ACStructure_Placeable* InPlaceable)
{
	InPlaceable->BroadcastExtinguish();
}

void ACSurvivorController::RequestRemoveProduceItemFromQueue_Implementation(class ACStructure_Placeable* InPlaceable, int32 InIndex)
{
	InPlaceable->BroadcastRemoveProduceItemFromQueue(InIndex);
}

void ACSurvivorController::RequestOpenDoor_Implementation(class ACStructure_Door* InDoor)
{
	InDoor->BroadcastOpenDoor();
}

void ACSurvivorController::RequestCloseDoor_Implementation(class ACStructure_Door* InDoor)
{
	InDoor->BroadcastCloseDoor();
} 

void ACSurvivorController::RequestSetRespawnLocationName_Implementation(class ACStructure_Placeable* InPlaceable, const FText& InText)
{
	InPlaceable->BroadcastSetRespawnLocationName(InText);
}

void ACSurvivorController::BroadcastDestroyPlayerLocation_Implementation()
{
	CDebug::Print("BroadcastDestroyPlayerLocation_Implementation Called");

	UCGameInstance* gameInstance = Cast<UCGameInstance>(GetWorld()->GetGameInstance());
	if (gameInstance)
	{
		//gameInstance->WorldMap->GetPlayerLocationPtr().Get()->RemoveFromParent();
		//gameInstance->WorldMap->GetPlayerLocationPtr().Get()->MarkPendingKill();
		if (gameInstance->WorldMap->GetPlayerLocationPtr().IsValid())
			gameInstance->WorldMap->GetPlayerLocationPtr().Get()->RemovePlayerLocation();
		
	}
}

void ACSurvivorController::RequestDestroyPlayerLocation_Implementation()
{
	BroadcastDestroyPlayerLocation();
}