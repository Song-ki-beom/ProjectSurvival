#include "ActorComponents/CBuildComponent.h"
#include "Character/CSurvivorController_BuildTest.h"
#include "Utility/CDebug.h"

UCBuildComponent::UCBuildComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCBuildComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCBuildComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCBuildComponent::SelectQ()
{
	ACSurvivorController_BuildTest* survivorController = Cast<ACSurvivorController_BuildTest>(GetWorld()->GetFirstPlayerController());
	if (IsValid(survivorController))
	{
		if (survivorController->GetIsBuildWidgetOn())
		{
			CDebug::Print("SelectQ");
			survivorController->ToggleBuildWidget();
		}
	}
	else
	{
		CDebug::Print("survivorController is not valid");
	}
}

void UCBuildComponent::SelectW()
{
	ACSurvivorController_BuildTest* survivorController = Cast<ACSurvivorController_BuildTest>(GetWorld()->GetFirstPlayerController());
	if (IsValid(survivorController))
	{
		if (survivorController->GetIsBuildWidgetOn())
		{
			CDebug::Print("SelectW");
			survivorController->ToggleBuildWidget();
		}
	}
	else
	{
		CDebug::Print("survivorController is not valid");
	}
}

void UCBuildComponent::SelectE()
{
	ACSurvivorController_BuildTest* survivorController = Cast<ACSurvivorController_BuildTest>(GetWorld()->GetFirstPlayerController());
	if (IsValid(survivorController))
	{
		if (survivorController->GetIsBuildWidgetOn())
		{
			CDebug::Print("SelectE");
			survivorController->ToggleBuildWidget();
		}
	}
	else
	{
		CDebug::Print("survivorController is not valid");
	}
}

void UCBuildComponent::SelectA()
{
	ACSurvivorController_BuildTest* survivorController = Cast<ACSurvivorController_BuildTest>(GetWorld()->GetFirstPlayerController());
	if (IsValid(survivorController))
	{
		if (survivorController->GetIsBuildWidgetOn())
		{
			CDebug::Print("SelectA");
			survivorController->ToggleBuildWidget();
		}
	}
	else
	{
		CDebug::Print("survivorController is not valid");
	}
}

void UCBuildComponent::SelectS()
{
	ACSurvivorController_BuildTest* survivorController = Cast<ACSurvivorController_BuildTest>(GetWorld()->GetFirstPlayerController());
	if (IsValid(survivorController))
	{
		if (survivorController->GetIsBuildWidgetOn())
		{
			CDebug::Print("SelectS");
			survivorController->ToggleBuildWidget();
		}
	}
	else
	{
		CDebug::Print("survivorController is not valid");
	}
}

void UCBuildComponent::SelectD()
{
	ACSurvivorController_BuildTest* survivorController = Cast<ACSurvivorController_BuildTest>(GetWorld()->GetFirstPlayerController());
	if (IsValid(survivorController))
	{
		if (survivorController->GetIsBuildWidgetOn())
		{
			CDebug::Print("SelectD");
			survivorController->ToggleBuildWidget();
		}
	}
	else
	{
		CDebug::Print("survivorController is not valid");
	}
}

void UCBuildComponent::SelectZ()
{
	ACSurvivorController_BuildTest* survivorController = Cast<ACSurvivorController_BuildTest>(GetWorld()->GetFirstPlayerController());
	if (IsValid(survivorController))
	{
		if (survivorController->GetIsBuildWidgetOn())
		{
			CDebug::Print("SelectZ");
			survivorController->ToggleBuildWidget();
		}
	}
	else
	{
		CDebug::Print("survivorController is not valid");
	}
}

void UCBuildComponent::SelectX()
{
	ACSurvivorController_BuildTest* survivorController = Cast<ACSurvivorController_BuildTest>(GetWorld()->GetFirstPlayerController());
	if (IsValid(survivorController))
	{
		if (survivorController->GetIsBuildWidgetOn())
		{
			CDebug::Print("SelectX");
			survivorController->ToggleBuildWidget();
		}
	}
	else
	{
		CDebug::Print("survivorController is not valid");
	}
}

void UCBuildComponent::SelectC()
{
	ACSurvivorController_BuildTest* survivorController = Cast<ACSurvivorController_BuildTest>(GetWorld()->GetFirstPlayerController());
	if (IsValid(survivorController))
	{
		if (survivorController->GetIsBuildWidgetOn())
		{
			CDebug::Print("SelectC");
			survivorController->ToggleBuildWidget();
		}
	}
	else
	{
		CDebug::Print("survivorController is not valid");
	}
}
