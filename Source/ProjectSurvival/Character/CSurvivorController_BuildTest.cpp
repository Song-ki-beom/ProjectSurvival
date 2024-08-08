#include "Character/CSurvivorController_BuildTest.h"
////////////////////////////////////////////////////
#include "ActorComponents/CBuildComponent.h"
#include "Widget/Build/CBuildWidget.h"
#include "Utility/CDebug.h"
#include "Character/CSurvivor_BuildTest.h"
#include "GameFramework/CharacterMovementComponent.h"

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
			CSurvivor_BuildTest->InputComponent->BindAction("Build", IE_Pressed, buildComponent, &UCBuildComponent::ToggleBuildWidget);
			CSurvivor_BuildTest->InputComponent->BindAction("SelectQ", IE_Pressed, buildComponent, &UCBuildComponent::SelectQ);
			CSurvivor_BuildTest->InputComponent->BindAction("SelectW", IE_Pressed, buildComponent, &UCBuildComponent::SelectW);
			CSurvivor_BuildTest->InputComponent->BindAction("SelectE", IE_Pressed, buildComponent, &UCBuildComponent::SelectE);
			CSurvivor_BuildTest->InputComponent->BindAction("SelectA", IE_Pressed, buildComponent, &UCBuildComponent::SelectA);
			CSurvivor_BuildTest->InputComponent->BindAction("SelectS", IE_Pressed, buildComponent, &UCBuildComponent::SelectS);
			CSurvivor_BuildTest->InputComponent->BindAction("SelectD", IE_Pressed, buildComponent, &UCBuildComponent::SelectD);
			CSurvivor_BuildTest->InputComponent->BindAction("SelectZ", IE_Pressed, buildComponent, &UCBuildComponent::SelectZ);
			CSurvivor_BuildTest->InputComponent->BindAction("SelectX", IE_Pressed, buildComponent, &UCBuildComponent::SelectX);
			CSurvivor_BuildTest->InputComponent->BindAction("SelectC", IE_Pressed, buildComponent, &UCBuildComponent::SelectC);
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
