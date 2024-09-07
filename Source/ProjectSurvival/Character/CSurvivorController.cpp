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

	static ConstructorHelpers::FObjectFinder<UDataTable> buildStructureDataFinder(TEXT("DataTable'/Game/PirateIsland/Include/Datas/Widget/Build/DT_BuildStructureInfo.DT_BuildStructureInfo'"));
	if (buildStructureDataFinder.Succeeded())
	{
		BuildStructureData = buildStructureDataFinder.Object;
		//CDebug::Print("buildStructureDataFinder Succeeded", FColor::Green);
	}
	else
		CDebug::Print("buildStructureDataFinder Failed", FColor::Red);

	static ConstructorHelpers::FClassFinder<UUserWidget> produceWidgetFinder(TEXT("WidgetBlueprint'/Game/PirateIsland/Include/Blueprints/Widget/Produce/WBP_CProduceWidget.WBP_CProduceWidget_C'"));
	if (produceWidgetFinder.Succeeded())
	{
		ProduceWidgetClass = produceWidgetFinder.Class;
	}
	else
		CDebug::Print("produceWidgetFinder Failed", FColor::Red);

	bIsBuildWidgetOn = false;
	bIsProduceWidgetOn = false;
}

void ACSurvivorController::BeginPlay()
{
	Super::BeginPlay();
	this->SetInputMode(FInputModeGameOnly());
	GetSurvivor();
	SetupInputFunction();
}

//void ACSurvivorController::OpenActorInventory(TSubclassOf<class UUserWidget> InClass)
//{
//	CDebug::Print("OpenActorInventory Called");
//	if (InClass)
//	{
//		UUserWidget* widgetInstance = CreateWidget<UUserWidget>(this, InClass);
//		if (widgetInstance)
//		{
//			UCInventoryPanel_WorkingBench* workingBenchWidget = Cast<UCInventoryPanel_WorkingBench>(widgetInstance);
//			if (workingBenchWidget)
//			{
//				ToggleMenu();
//				workingBenchWidget->AddToViewport(5);
//				workingBenchWidget->SetVisibility(ESlateVisibility::Visible);
//			}
//			else
//				CDebug::Print("workingBenchWidget is not Valid");
//		}
//		else
//			CDebug::Print("widgetInstance is not Valid");
//	}
//}

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
		InputComponent->BindKey(EKeys::P, IE_Pressed, this, &ACSurvivorController::TestP);
		InputComponent->BindAction("Inventory", IE_Pressed, this, &ACSurvivorController::ShowWidget);
		InputComponent->BindAction("MouseWheelUp", IE_Pressed, this,&ACSurvivorController::HandleMouseWheelUp);
		InputComponent->BindAction("MouseWheelDown", IE_Pressed,this, &ACSurvivorController::HandleMouseWheelDown);

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

void ACSurvivorController::TestP()
{
	// 빌드 위젯 이미지변경하는 임시함수
	// 우클릭해서 등록할때 구조물 이름 및 저장위치 정보 가져와야함
	CDebug::Print("Test Called");
	SetupBuildWidget();
	FBuildStructureInfo* structureInfo = BuildStructureData->FindRow<FBuildStructureInfo>("WoodFoundation", TEXT("WoodFoundation"));
	UTexture2D* texture = structureInfo->StructureTexture;
	TSubclassOf<ACStructure> structureClass = structureInfo->StructureClass;
	EBuildStructureElement structureElem = structureInfo->StructureElement;
	BuildWidget->SaveStructureInfo(ESelectedStructure::Q, texture, structureClass, structureElem);

	FBuildStructureInfo* structureInfo2 = BuildStructureData->FindRow<FBuildStructureInfo>("WoodWall", TEXT("WoodWall"));
	UTexture2D* texture2 = structureInfo2->StructureTexture;
	TSubclassOf<ACStructure> structureClass2 = structureInfo2->StructureClass;
	EBuildStructureElement structureElem2 = structureInfo2->StructureElement;
	BuildWidget->SaveStructureInfo(ESelectedStructure::W, texture2, structureClass2, structureElem2);

	FBuildStructureInfo* structureInfo3 = BuildStructureData->FindRow<FBuildStructureInfo>("WoodCeiling", TEXT("WoodCeiling"));
	UTexture2D* texture3 = structureInfo3->StructureTexture;
	TSubclassOf<ACStructure> structureClass3 = structureInfo3->StructureClass;
	EBuildStructureElement structureElem3 = structureInfo3->StructureElement;
	BuildWidget->SaveStructureInfo(ESelectedStructure::E, texture3, structureClass3, structureElem3);

	FBuildStructureInfo* structureInfo4 = BuildStructureData->FindRow<FBuildStructureInfo>("WoodRamp", TEXT("WoodRamp"));
	UTexture2D* texture4 = structureInfo4->StructureTexture;
	TSubclassOf<ACStructure> structureClass4 = structureInfo4->StructureClass;
	EBuildStructureElement structureElem4 = structureInfo4->StructureElement;
	BuildWidget->SaveStructureInfo(ESelectedStructure::A, texture4, structureClass4, structureElem4);

	FBuildStructureInfo* structureInfo5 = BuildStructureData->FindRow<FBuildStructureInfo>("WoodDoorFrame", TEXT("WoodDoorFrame"));
	UTexture2D* texture5 = structureInfo5->StructureTexture;
	TSubclassOf<ACStructure> structureClass5 = structureInfo5->StructureClass;
	EBuildStructureElement structureElem5 = structureInfo5->StructureElement;
	BuildWidget->SaveStructureInfo(ESelectedStructure::S, texture5, structureClass5, structureElem5);

	FBuildStructureInfo* structureInfo6 = BuildStructureData->FindRow<FBuildStructureInfo>("WoodDoor", TEXT("WoodDoor"));
	UTexture2D* texture6 = structureInfo6->StructureTexture;
	TSubclassOf<ACStructure> structureClass6 = structureInfo6->StructureClass;
	EBuildStructureElement structureElem6 = structureInfo6->StructureElement;
	BuildWidget->SaveStructureInfo(ESelectedStructure::D, texture6, structureClass6, structureElem6);

	FBuildStructureInfo* structureInfo7 = BuildStructureData->FindRow<FBuildStructureInfo>("WoodStair", TEXT("WoodStair"));
	UTexture2D* texture7 = structureInfo7->StructureTexture;
	TSubclassOf<ACStructure> structureClass7 = structureInfo7->StructureClass;
	EBuildStructureElement structureElem7 = structureInfo7->StructureElement;
	BuildWidget->SaveStructureInfo(ESelectedStructure::Z, texture7, structureClass7, structureElem7);

	FBuildStructureInfo* structureInfo8 = BuildStructureData->FindRow<FBuildStructureInfo>("WorkingBench", TEXT("WorkingBench"));
	UTexture2D* texture8 = structureInfo8->StructureTexture;
	TSubclassOf<ACStructure> structureClass8 = structureInfo8->StructureClass;
	EBuildStructureElement structureElem8 = structureInfo8->StructureElement;
	BuildWidget->SaveStructureInfo(ESelectedStructure::X, texture8, structureClass8, structureElem8);
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
	//ACSurvivor* controlledCharacter = Cast<ACSurvivor>(this->GetCharacter());

	//if (controlledCharacter)
	//{
	//	controlledCharacter->HoldAxe();
	//}

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

//void ACSurvivorController::ToggleProduceWidget()
//{
//	if (IsValid(ProduceWidget))
//	{
//		if (bIsProduceWidgetOn)
//		{
//			CDebug::Print("Off Produce Widget");
//			bIsProduceWidgetOn = false;
//			ProduceWidget->SetVisibility(ESlateVisibility::Collapsed);
//			//ProduceWidget->bIsFocusable = false;
//			this->SetInputMode(FInputModeGameOnly());
//		}
//		else
//		{
//			CDebug::Print("On Produce Widget");
//			bIsProduceWidgetOn = true;
//			ProduceWidget->SetVisibility(ESlateVisibility::Visible);
//			//ProduceWidget->bIsFocusable = true;
//			this->SetInputMode(FInputModeUIOnly());
//		}
//	}
//	else
//	{
//		ProduceWidget = CreateWidget<UCProduceWidget>(this, ProduceWidgetClass);
//		ProduceWidget->OnProduceWidgetToggled.AddUObject(this, &ACSurvivorController::ToggleProduceWidget);
//		ProduceWidget->AddToViewport();
//		bIsProduceWidgetOn = true;
//		ProduceWidget->bIsFocusable = true;
//		this->SetInputMode(FInputModeUIOnly());
//	}
//}
