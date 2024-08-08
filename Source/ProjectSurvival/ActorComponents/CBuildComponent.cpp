#include "ActorComponents/CBuildComponent.h"
#include "Character/CSurvivorController_BuildTest.h"
#include "Widget/Build/CBuildWidget.h"
#include "Utility/CDebug.h"

UCBuildComponent::UCBuildComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

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

void UCBuildComponent::BeginPlay()
{
	Super::BeginPlay();
	ACSurvivorController_BuildTest* survivorController = Cast<ACSurvivorController_BuildTest>(GetWorld()->GetFirstPlayerController());
	BuildWidget = survivorController->GetBuildWidget();
	if (survivorController)
		CDebug::Print("Build Widget Valid");
	else
		CDebug::Print("Build Widget is not Vaild");
}

void UCBuildComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCBuildComponent::ToggleBuildWidget()
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

void UCBuildComponent::SelectQ()
{
	if (bIsBuildWidgetOn)
	{
		CDebug::Print("SelectQ");
		ToggleBuildWidget();
	}
}

void UCBuildComponent::SelectW()
{
	if (bIsBuildWidgetOn)
	{
		CDebug::Print("SelectW");
		ToggleBuildWidget();
	}
}

void UCBuildComponent::SelectE()
{
	if (bIsBuildWidgetOn)
	{
		CDebug::Print("SelectE");
		ToggleBuildWidget();
	}
}

void UCBuildComponent::SelectA()
{
	if (bIsBuildWidgetOn)
	{
		CDebug::Print("SelectA");
		ToggleBuildWidget();
	}
}

void UCBuildComponent::SelectS()
{
	if (bIsBuildWidgetOn)
	{
		CDebug::Print("SelectS");
		ToggleBuildWidget();
	}
}

void UCBuildComponent::SelectD()
{
	if (bIsBuildWidgetOn)
	{
		CDebug::Print("SelectD");
		ToggleBuildWidget();
	}
}

void UCBuildComponent::SelectZ()
{
	if (bIsBuildWidgetOn)
	{
		CDebug::Print("SelectZ");
		ToggleBuildWidget();
	}
}

void UCBuildComponent::SelectX()
{
	if (bIsBuildWidgetOn)
	{
		CDebug::Print("SelectX");
		ToggleBuildWidget();
	}
}

void UCBuildComponent::SelectC()
{
	if (bIsBuildWidgetOn)
	{
		CDebug::Print("SelectC");
		ToggleBuildWidget();
	}
}
