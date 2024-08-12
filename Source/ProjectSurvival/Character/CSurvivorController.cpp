// Fill out your copyright notice in the Description page of Project Settings.


#include "CSurvivorController.h"
#include "CSurvivor.h"
#include "Engine/Engine.h"
#include "InputCoreTypes.h"
#include "GameFramework/Actor.h"
#include "ActorComponents/CBuildComponent.h"
#include "ActorComponents/CMovingComponent.h"
#include "Widget/Build/CBuildWidget.h"
#include "Utility/CDebug.h"

ACSurvivorController::ACSurvivorController()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> buildWidgetFinder(TEXT("WidgetBlueprint'/Game/PirateIsland/Include/Blueprints/Widget/Build/WBP_BuildWidget.WBP_BuildWidget_C'"));
	if (buildWidgetFinder.Succeeded())
	{
		BuildWidgetClass = buildWidgetFinder.Class;
		CDebug::Log("buildWidgetFinder Succeeded");
	}
	else
		CDebug::Log("buildWidgetFinder Failed");

	static ConstructorHelpers::FObjectFinder<UDataTable> buildStructureDataFinder(TEXT("DataTable'/Game/PirateIsland/Include/Datas/Widget/Build/DT_BuildStructureInfo.DT_BuildStructureInfo'"));
	if (buildStructureDataFinder.Succeeded())
	{
		BuildStructureData = buildStructureDataFinder.Object;
	}
	else
	{
		CDebug::Log("buildStructureDataFinder Failed");
	}

	bIsBuildWidgetOn = false;
}

void ACSurvivorController::BeginPlay()
{
	Super::BeginPlay();
	this->SetInputMode(FInputModeGameOnly());
	GetSurvivor();
	SetupBuildWidget();
	SetupInputFunction();
}

void ACSurvivorController::GetSurvivor()
{
	CSurvivor = Cast<ACSurvivor>(this->GetCharacter());
}

void ACSurvivorController::SetupBuildWidget()
{
	BuildWidget = CreateWidget<UCBuildWidget>(this, BuildWidgetClass);
	BuildWidget->AddToViewport();
	BuildWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void ACSurvivorController::SetupInputFunction()
{
	if (IsValid(CSurvivor))
	{
		InputComponent->BindKey(EKeys::T, IE_Pressed, this, &ACSurvivorController::HoldAxe);
		InputComponent->BindKey(EKeys::LeftMouseButton, IE_Pressed, this, &ACSurvivorController::DoAction);
		InputComponent->BindKey(EKeys::RightMouseButton, IE_Pressed, this, &ACSurvivorController::SubAction);
		InputComponent->BindAction("Build", IE_Pressed, this, &ACSurvivorController::ToggleBuildWidget);
		InputComponent->BindAction("SelectQ", IE_Pressed, this, &ACSurvivorController::SelectQ);
		InputComponent->BindAction("SelectW", IE_Pressed, this, &ACSurvivorController::SelectW);
		InputComponent->BindAction("SelectE", IE_Pressed, this, &ACSurvivorController::SelectE);
		InputComponent->BindAction("SelectA", IE_Pressed, this, &ACSurvivorController::SelectA);
		InputComponent->BindAction("SelectS", IE_Pressed, this, &ACSurvivorController::SelectS);
		InputComponent->BindAction("SelectD", IE_Pressed, this, &ACSurvivorController::SelectD);
		InputComponent->BindAction("SelectZ", IE_Pressed, this, &ACSurvivorController::SelectZ);
		InputComponent->BindAction("SelectX", IE_Pressed, this, &ACSurvivorController::SelectX);
		InputComponent->BindAction("SelectC", IE_Pressed, this, &ACSurvivorController::SelectC);
		InputComponent->BindKey(EKeys::P, IE_Pressed, this, &ACSurvivorController::TestP);

		UCMovingComponent* movingComponent = CSurvivor->GetMovingComponent();
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
	CDebug::Print("Call");

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

void ACSurvivorController::SelectQ()
{
	if (bIsBuildWidgetOn)
	{
		if (IsValid(BuildWidget->GetStructureClass(ESelectedStructure::Q)))
		{
			TSubclassOf<ACStructure> structureClass = BuildWidget->GetStructureClass(ESelectedStructure::Q);
			EBuildStructureElement structureElement = BuildWidget->GetStructureElement(ESelectedStructure::Q);
			CSurvivor->SelectStructure(ESelectedStructure::Q, structureClass, structureElement);
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
		//CSurvivor_BuildTest->SelectStructure(ESelectedStructure::W);
		ToggleBuildWidget();
	}
}

void ACSurvivorController::SelectE()
{
	if (bIsBuildWidgetOn)
	{
		//CSurvivor_BuildTest->SelectStructure(ESelectedStructure::E);
		ToggleBuildWidget();
	}
}

void ACSurvivorController::SelectA()
{
	if (bIsBuildWidgetOn)
	{
		//CSurvivor_BuildTest->SelectStructure(ESelectedStructure::A);
		ToggleBuildWidget();
	}
}

void ACSurvivorController::SelectS()
{
	if (bIsBuildWidgetOn)
	{
		//CSurvivor_BuildTest->SelectStructure(ESelectedStructure::S);
		ToggleBuildWidget();
	}
}

void ACSurvivorController::SelectD()
{
	if (bIsBuildWidgetOn)
	{
		//CSurvivor_BuildTest->SelectStructure(ESelectedStructure::D);
		ToggleBuildWidget();
	}
}

void ACSurvivorController::SelectZ()
{
	if (bIsBuildWidgetOn)
	{
		//CSurvivor_BuildTest->SelectStructure(ESelectedStructure::Z);
		ToggleBuildWidget();
	}
}

void ACSurvivorController::SelectX()
{
	if (bIsBuildWidgetOn)
	{
		//CSurvivor_BuildTest->SelectStructure(ESelectedStructure::X);
		ToggleBuildWidget();
	}
}

void ACSurvivorController::SelectC()
{
	if (bIsBuildWidgetOn)
	{
		//CSurvivor_BuildTest->SelectStructure(ESelectedStructure::C);
		ToggleBuildWidget();
	}
}

void ACSurvivorController::TestP()
{
	// 빌드 위젯 이미지변경하는 임시함수
	// 우클릭해서 등록할때 구조물 이름 및 저장위치 정보 가져와야함
	CDebug::Print("Test Called");
	FBuildStructureInfo* structureInfo = BuildStructureData->FindRow<FBuildStructureInfo>("WoodFoundation", TEXT("WoodFoundation"));
	UTexture2D* texture = structureInfo->StructureTexture;
	TSubclassOf<ACStructure> structureClass = structureInfo->StructureClass;
	EBuildStructureElement structureElem = structureInfo->StructureElement;
	BuildWidget->SaveStructureInfo(ESelectedStructure::Q, texture, structureClass, structureElem);
}

void ACSurvivorController::DoAction()
{
	if (CSurvivor)
	{
		if (CSurvivor->GetBuildComponent()->CheckIsBuilding())
		{
			CSurvivor->Build();
			return;
		}
		CSurvivor->DoAction();
	}
}

void ACSurvivorController::SubAction()
{
	if (CSurvivor)
	{
		if (CSurvivor->GetBuildComponent()->CheckIsBuilding())
		{
			CSurvivor->CancleBuild();
			return;
		}
		CSurvivor->SubAction();
	}
}

void ACSurvivorController::HoldAxe()
{
	//ACSurvivor* controlledCharacter = Cast<ACSurvivor>(this->GetCharacter());

	//if (controlledCharacter)
	//{
	//	controlledCharacter->HoldAxe();
	//}

	if (CSurvivor)
	{
		CSurvivor->HoldAxe();
	}
}