#include "Lobby/CLobbySurvivorController.h"
#include "Lobby/CCustomizeWidget.h"
#include "Lobby/CWaitingWidget.h"
#include "Lobby/CSurvivorName.h"
#include "Lobby/CLobbySurvivor.h"
#include "Net/UnrealNetwork.h"

ACLobbySurvivorController::ACLobbySurvivorController()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> customizeWidgetFinder(TEXT("WidgetBlueprint'/Game/PirateIsland/Include/Blueprints/Widget/Lobby/WBP_CCustomizeWidget.WBP_CCustomizeWidget_C'"));
	if (customizeWidgetFinder.Succeeded())
		CustomizeClass = customizeWidgetFinder.Class;
	else
		UE_LOG(LogTemp, Warning, TEXT("customizeWidgetFinder Failed - ACLobbySurvivorController"));

	LobbySurvivor = Cast<ACLobbySurvivor>(this->GetPawn());

	static ConstructorHelpers::FClassFinder<UUserWidget> waitingWidgetFinder(TEXT("WidgetBlueprint'/Game/PirateIsland/Include/Blueprints/Widget/Lobby/WBP_CWaitingWidget.WBP_CWaitingWidget_C'"));
	if (waitingWidgetFinder.Succeeded())
		WaitingClass = waitingWidgetFinder.Class;
	else
		UE_LOG(LogTemp, Warning, TEXT("waitingWidgetFinder Failed - ACLobbySurvivorController"));
}

void ACLobbySurvivorController::BeginPlay()
{

}

void ACLobbySurvivorController::LoadCustomizeWidget()
{
	CustomizeWidget = CreateWidget<UCCustomizeWidget>(this, CustomizeClass);
	if (IsValid(CustomizeWidget))
		CustomizeWidget->SetUpWidget();
	else
		UE_LOG(LogTemp, Warning, TEXT("CustomizeWidget is not valid"));
}

void ACLobbySurvivorController::LoadWaitingWidget()
{
	WaitingWidget = CreateWidget<UCWaitingWidget>(this, WaitingClass);
	if (IsValid(WaitingWidget))
		WaitingWidget->SetUpWidget();
	else
		UE_LOG(LogTemp, Warning, TEXT("WaitingWidget is not valid"));
}

