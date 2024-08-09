#include "ActorComponents/CBuildComponent.h"
#include "Character/CSurvivorController_BuildTest.h"
#include "Widget/Build/CBuildWidget.h"
#include "Utility/CDebug.h"

UCBuildComponent::UCBuildComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
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

void UCBuildComponent::SelectQ()
{
	CDebug::Print("SelectQ");
}

void UCBuildComponent::SelectW()
{
	CDebug::Print("SelectW");
}

void UCBuildComponent::SelectE()
{
	CDebug::Print("SelectE");
}

void UCBuildComponent::SelectA()
{
	CDebug::Print("SelectA");
}

void UCBuildComponent::SelectS()
{
	CDebug::Print("SelectS");
}

void UCBuildComponent::SelectD()
{
	CDebug::Print("SelectD");
}

void UCBuildComponent::SelectZ()
{
	CDebug::Print("SelectZ");
}

void UCBuildComponent::SelectX()
{
	CDebug::Print("SelectX");
}

void UCBuildComponent::SelectC()
{
	CDebug::Print("SelectC");
}
