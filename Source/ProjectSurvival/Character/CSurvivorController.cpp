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
#include "Widget/Produce/CProduceWidget.h"
#include "Widget/Inventory/CInventoryPanel_WorkingBench.h"
#include "Utility/CDebug.h"
#include "CGameInstance.h"
#include "Build/CStructure_Placeable.h"
#include "Widget/CMainHUD.h"
#include "Widget/Chatting/CChattingBox.h"
#include "Widget/Inventory/CQuickSlot.h"
#include "Components/SizeBox.h"
#include "Widget/Inventory/CInventoryItemSlot.h"

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

	//static ConstructorHelpers::FObjectFinder<UDataTable> buildStructureDataFinder(TEXT("DataTable'/Game/PirateIsland/Include/Datas/Widget/Build/DT_BuildStructureInfo.DT_BuildStructureInfo'"));
	//if (buildStructureDataFinder.Succeeded())
	//{
	//	BuildStructureData = buildStructureDataFinder.Object;
	//	//CDebug::Print("buildStructureDataFinder Succeeded", FColor::Green);
	//}
	//else
	//	CDebug::Print("buildStructureDataFinder Failed", FColor::Red);

	bIsBuildWidgetOn = false;
	bIsProduceWidgetOn = false;
}

void ACSurvivorController::BeginPlay()
{
	Super::BeginPlay();
	this->SetInputMode(FInputModeGameOnly());
	GetSurvivor();
	SetupInputFunction();
	//SetupBuildWidget();
}

void ACSurvivorController::GetSurvivor()
{
	Survivor = Cast<ACSurvivor>(this->GetCharacter());
}

void ACSurvivorController::SetupBuildWidget()
{
	if (!BuildWidget)
	{
		BuildWidget = CreateWidget<UCBuildWidget>(this, BuildWidgetClass);
		BuildWidget->AddToViewport();
		BuildWidget->SetVisibility(ESlateVisibility::Collapsed);
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
		CDebug::Print("CSurvivor is not valid");
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
			CDebug::Print("Off Build");
			bIsBuildWidgetOn = false;
			BuildWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			CDebug::Print("On Build");
			bIsBuildWidgetOn = true;
			BuildWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		BuildWidget = CreateWidget<UCBuildWidget>(this, BuildWidgetClass);
		BuildWidget->AddToViewport();
		bIsBuildWidgetOn = true;
	}
}

void ACSurvivorController::SelectQ()
{
	if (bIsBuildWidgetOn)
	{
		if (IsValid(BuildWidget->GetStructureClass(ESelectedStructure::Q)))
		{
			TSubclassOf<ACStructure> structureClass = BuildWidget->GetStructureClass(ESelectedStructure::Q);
			EBuildStructureElement structureElement = BuildWidget->GetStructureElement(ESelectedStructure::Q);
			Survivor->SelectStructure(ESelectedStructure::Q, structureClass, structureElement);
			ToggleBuildWidget();
		}
		else
		{
			CDebug::Print("InValid Class or Number 0");
		}
	}
}

void ACSurvivorController::SelectW()
{
	if (bIsBuildWidgetOn)
	{
		if (IsValid(BuildWidget->GetStructureClass(ESelectedStructure::W)))
		{
			TSubclassOf<ACStructure> structureClass = BuildWidget->GetStructureClass(ESelectedStructure::W);
			EBuildStructureElement structureElement = BuildWidget->GetStructureElement(ESelectedStructure::W);
			Survivor->SelectStructure(ESelectedStructure::W, structureClass, structureElement);
			ToggleBuildWidget();
		}
		else
		{
			CDebug::Print("InValid Class or Number 0");
		}
	}
}

void ACSurvivorController::SelectE()
{
	if (bIsBuildWidgetOn)
	{
		if (IsValid(BuildWidget->GetStructureClass(ESelectedStructure::E)))
		{
			TSubclassOf<ACStructure> structureClass = BuildWidget->GetStructureClass(ESelectedStructure::E);
			EBuildStructureElement structureElement = BuildWidget->GetStructureElement(ESelectedStructure::E);
			Survivor->SelectStructure(ESelectedStructure::E, structureClass, structureElement);
			ToggleBuildWidget();
		}
		else
		{
			CDebug::Print("InValid Class or Number 0");
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
			TSubclassOf<ACStructure> structureClass = BuildWidget->GetStructureClass(ESelectedStructure::A);
			EBuildStructureElement structureElement = BuildWidget->GetStructureElement(ESelectedStructure::A);
			Survivor->SelectStructure(ESelectedStructure::A, structureClass, structureElement);
			ToggleBuildWidget();
		}
		else
		{
			CDebug::Print("InValid Class or Number 0");
		}
	}
}

void ACSurvivorController::SelectS()
{
	if (bIsBuildWidgetOn)
	{
		if (IsValid(BuildWidget->GetStructureClass(ESelectedStructure::S)))
		{
			TSubclassOf<ACStructure> structureClass = BuildWidget->GetStructureClass(ESelectedStructure::S);
			EBuildStructureElement structureElement = BuildWidget->GetStructureElement(ESelectedStructure::S);
			Survivor->SelectStructure(ESelectedStructure::S, structureClass, structureElement);
			ToggleBuildWidget();
		}
		else
		{
			CDebug::Print("InValid Class or Number 0");
		}
	}
}

void ACSurvivorController::SelectD()
{
	if (bIsBuildWidgetOn)
	{
		if (IsValid(BuildWidget->GetStructureClass(ESelectedStructure::D)))
		{
			TSubclassOf<ACStructure> structureClass = BuildWidget->GetStructureClass(ESelectedStructure::D);
			EBuildStructureElement structureElement = BuildWidget->GetStructureElement(ESelectedStructure::D);
			Survivor->SelectStructure(ESelectedStructure::D, structureClass, structureElement);
			ToggleBuildWidget();
		}
		else
		{
			CDebug::Print("InValid Class or Number 0");
		}
	}
}

void ACSurvivorController::SelectZ()
{
	if (bIsBuildWidgetOn)
	{
		if (IsValid(BuildWidget->GetStructureClass(ESelectedStructure::Z)))
		{
			TSubclassOf<ACStructure> structureClass = BuildWidget->GetStructureClass(ESelectedStructure::Z);
			EBuildStructureElement structureElement = BuildWidget->GetStructureElement(ESelectedStructure::Z);
			Survivor->SelectStructure(ESelectedStructure::Z, structureClass, structureElement);
			ToggleBuildWidget();
		}
		else
		{
			CDebug::Print("InValid Class or Number 0");
		}
	}
}

void ACSurvivorController::SelectX()
{
	if (bIsBuildWidgetOn)
	{
		if (IsValid(BuildWidget->GetStructureClass(ESelectedStructure::X)))
		{
			TSubclassOf<ACStructure> structureClass = BuildWidget->GetStructureClass(ESelectedStructure::X);
			EBuildStructureElement structureElement = BuildWidget->GetStructureElement(ESelectedStructure::X);
			Survivor->SelectStructure(ESelectedStructure::X, structureClass, structureElement);
			ToggleBuildWidget();
		}
		else
		{
			CDebug::Print("InValid Class or Number 0");
		}
	}
}

void ACSurvivorController::SelectC()
{
	if (bIsBuildWidgetOn)
	{
		if (IsValid(BuildWidget->GetStructureClass(ESelectedStructure::C)))
		{
			TSubclassOf<ACStructure> structureClass = BuildWidget->GetStructureClass(ESelectedStructure::C);
			EBuildStructureElement structureElement = BuildWidget->GetStructureElement(ESelectedStructure::C);
			Survivor->SelectStructure(ESelectedStructure::C, structureClass, structureElement);
			ToggleBuildWidget();
		}
		else
		{
			CDebug::Print("InValid Class or Number 0");
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

void ACSurvivorController::PressQuickSlot1()
{
	ACMainHUD* mainHUD = Cast<ACMainHUD>(this->GetHUD());
	if (!mainHUD)
		return;

	UCQuickSlot* quickSlot = mainHUD->GetQuickSlotWidget();
	if (!quickSlot)
		return;

	USizeBox* sizeBox = quickSlot->GetSizeBoxArray()[0];
	if (!sizeBox)
		return;

	UCInventoryItemSlot* quickSlotItemWidget = Cast<UCInventoryItemSlot>(sizeBox->GetChildAt(0));
	if (!quickSlotItemWidget)
		return;

	UCItemBase* quickSlotItem = quickSlotItemWidget->GetItemReference();
	if (!quickSlotItem)
		return;

	if (quickSlotItem->ItemType == EItemType::Hunt)
		CDebug::Print("quickSlot1 Item ID : ", quickSlotItem->ID, FColor::Blue);

	// ID나 여러타입에 따른 스위치문함수 필요
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

void ACSurvivorController::RequestAddItem_Implementation(FName ItemID, int32 InQuantity, class ACStructure_Placeable* InPlaceable, FItemNumericData InNumericData, EItemType ItemType)
{
	InPlaceable->PerformAddItem(ItemID, InQuantity, InNumericData, ItemType);
}

void ACSurvivorController::RequestSwapItem_Implementation(int32 idxBase, int32  idxDrag, class ACStructure_Placeable* InPlaceable)
{
	InPlaceable->PerformSwapItem(idxBase, idxDrag);
};

void ACSurvivorController::RequestSortInfoWidget_Implementation(class ACStructure_Placeable* InPlaceable)
{
	InPlaceable->PerformSortInfoWidget();
}

void ACSurvivorController::RequestSplitItem_Implementation(int32 ItemIdx, int32 AmountToSplit, class ACStructure_Placeable* InPlaceable)
{
	InPlaceable->PerformSplitItem(ItemIdx, AmountToSplit);

}

void ACSurvivorController::RequestCombineItem_Implementation(int32 idxBase, int32  idxDrag, class ACStructure_Placeable* InPlaceable)
{
	InPlaceable->PerformCombineItem(idxBase, idxDrag);
}

void ACSurvivorController::RequestRemoveItem_Implementation(int32 idxRemove, class ACStructure_Placeable* InPlaceable)
{
	InPlaceable->PerformRemoveItem(idxRemove);
}

void ACSurvivorController::RequestRemoveAmountOfItem_Implementation(int32 idxRemove, int32 AmountToRemove, class ACStructure_Placeable* InPlaceable)
{
	InPlaceable->PerformRemoveAmountOfItem(idxRemove, AmountToRemove);
}

void ACSurvivorController::RequestAddProduceItemToQueue_Implementation(FName ItemID, class ACStructure_Placeable* InPlaceable)
{
	InPlaceable->BroadcastAddProduceItemToQueue(ItemID, InPlaceable);
}