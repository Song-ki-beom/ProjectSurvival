#include "Character/CSurvivorController_BuildTest.h"
////////////////////////////////////////////////////
#include "Widget/Build/CBuildWidget.h"
#include "Utility/CDebug.h"

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

	bIsBuildWidgetOn = false;
}

void ACSurvivorController_BuildTest::BeginPlay()
{
	Super::BeginPlay();
	SetupBuildWidget();
}

void ACSurvivorController_BuildTest::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (IsValid(InputComponent))
	{
		////////////////////////////////////////////////////
		InputComponent->BindAction("Build", IE_Pressed, this, &ACSurvivorController_BuildTest::ToggleBuildWidget);
	}
	else
	{
		CDebug::Print("InputComponent is not valid");
	}
}

void ACSurvivorController_BuildTest::SetupBuildWidget()
{
	BuildWidget = CreateWidget<UCBuildWidget>(this, BuildWidgetClass);
	BuildWidget->AddToViewport();
	BuildWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void ACSurvivorController_BuildTest::ToggleBuildWidget()
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