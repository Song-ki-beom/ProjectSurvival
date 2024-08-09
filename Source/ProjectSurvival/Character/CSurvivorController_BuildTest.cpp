#include "Character/CSurvivorController_BuildTest.h"
////////////////////////////////////////////////////
#include "ActorComponents/CBuildComponent.h"
#include "Widget/Build/CBuildWidget.h"
#include "Utility/CDebug.h"
#include "Character/CSurvivor_BuildTest.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomDataType/BuildStructureDataType.h"

ACSurvivorController_BuildTest::ACSurvivorController_BuildTest()
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

void ACSurvivorController_BuildTest::BeginPlay()
{
	Super::BeginPlay();
	GetSurvivor();
	SetupBuildWidget();
	SetupBuildComponentFunction();
}

void ACSurvivorController_BuildTest::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void ACSurvivorController_BuildTest::GetSurvivor()
{
	CSurvivor_BuildTest = Cast<ACSurvivor_BuildTest>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

void ACSurvivorController_BuildTest::SetupBuildWidget()
{
	BuildWidget = CreateWidget<UCBuildWidget>(this, BuildWidgetClass);
	BuildWidget->AddToViewport();
	BuildWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void ACSurvivorController_BuildTest::SetupBuildComponentFunction()
{
	if (IsValid(CSurvivor_BuildTest))
	{
		UCBuildComponent* buildComponent = CSurvivor_BuildTest->GetBuildComponent();
		if (IsValid(buildComponent))
		{
			CDebug::Print("buildComponent is valid");
			CSurvivor_BuildTest->InputComponent->BindAction("Build", IE_Pressed, this, &ACSurvivorController_BuildTest::ToggleBuildWidget);
			CSurvivor_BuildTest->InputComponent->BindAction("SelectQ", IE_Pressed, this, &ACSurvivorController_BuildTest::SelectQ);
			CSurvivor_BuildTest->InputComponent->BindAction("SelectW", IE_Pressed, this, &ACSurvivorController_BuildTest::SelectW);
			CSurvivor_BuildTest->InputComponent->BindAction("SelectE", IE_Pressed, this, &ACSurvivorController_BuildTest::SelectE);
			CSurvivor_BuildTest->InputComponent->BindAction("SelectA", IE_Pressed, this, &ACSurvivorController_BuildTest::SelectA);
			CSurvivor_BuildTest->InputComponent->BindAction("SelectS", IE_Pressed, this, &ACSurvivorController_BuildTest::SelectS);
			CSurvivor_BuildTest->InputComponent->BindAction("SelectD", IE_Pressed, this, &ACSurvivorController_BuildTest::SelectD);
			CSurvivor_BuildTest->InputComponent->BindAction("SelectZ", IE_Pressed, this, &ACSurvivorController_BuildTest::SelectZ);
			CSurvivor_BuildTest->InputComponent->BindAction("SelectX", IE_Pressed, this, &ACSurvivorController_BuildTest::SelectX);
			CSurvivor_BuildTest->InputComponent->BindAction("SelectC", IE_Pressed, this, &ACSurvivorController_BuildTest::SelectC);
			CSurvivor_BuildTest->InputComponent->BindKey(EKeys::P, IE_Pressed, this, &ACSurvivorController_BuildTest::TestP);
		}
		else
		{
			CDebug::Print("buildComponent is not valid");
		}
	}
	else
	{
		CDebug::Print("CSurvivor_BuildTest is not valid");
	}
}

void ACSurvivorController_BuildTest::ToggleBuildWidget()
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

void ACSurvivorController_BuildTest::SelectQ()
{
	if (bIsBuildWidgetOn)
	{
		//CSurvivor_BuildTest->SelectStructure(ESelectedStructure::Q);
		ToggleBuildWidget();
	}
}

void ACSurvivorController_BuildTest::SelectW()
{
	if (bIsBuildWidgetOn)
	{
		//CSurvivor_BuildTest->SelectStructure(ESelectedStructure::W);
		ToggleBuildWidget();
	}
}

void ACSurvivorController_BuildTest::SelectE()
{
	if (bIsBuildWidgetOn)
	{
		//CSurvivor_BuildTest->SelectStructure(ESelectedStructure::E);
		ToggleBuildWidget();
	}
}

void ACSurvivorController_BuildTest::SelectA()
{
	if (bIsBuildWidgetOn)
	{
		//CSurvivor_BuildTest->SelectStructure(ESelectedStructure::A);
		ToggleBuildWidget();
	}
}

void ACSurvivorController_BuildTest::SelectS()
{
	if (bIsBuildWidgetOn)
	{
		//CSurvivor_BuildTest->SelectStructure(ESelectedStructure::S);
		ToggleBuildWidget();
	}
}

void ACSurvivorController_BuildTest::SelectD()
{
	if (bIsBuildWidgetOn)
	{
		//CSurvivor_BuildTest->SelectStructure(ESelectedStructure::D);
		ToggleBuildWidget();
	}
}

void ACSurvivorController_BuildTest::SelectZ()
{
	if (bIsBuildWidgetOn)
	{
		//CSurvivor_BuildTest->SelectStructure(ESelectedStructure::Z);
		ToggleBuildWidget();
	}
}

void ACSurvivorController_BuildTest::SelectX()
{
	if (bIsBuildWidgetOn)
	{
		//CSurvivor_BuildTest->SelectStructure(ESelectedStructure::X);
		ToggleBuildWidget();
	}
}

void ACSurvivorController_BuildTest::SelectC()
{
	if (bIsBuildWidgetOn)
	{
		//CSurvivor_BuildTest->SelectStructure(ESelectedStructure::C);
		ToggleBuildWidget();
	}
}

void ACSurvivorController_BuildTest::TestP()
{
	// 빌드 위젯 이미지변경하는 임시함수
	// 우클릭해서 등록할때 구조물 이름 및 저장위치 정보 가져와야함
	FBuildStructureInfo* structureInfo = BuildStructureData->FindRow<FBuildStructureInfo>("WoodFoundation", TEXT("WoodFoundation"));
	UTexture2D* texture = structureInfo->StructureTexture;
	TSubclassOf<ACStructure> structureClass = structureInfo->StructureClass;
	BuildWidget->SaveStructureInfo(ESelectedStructure::Q, texture, structureClass);
}
