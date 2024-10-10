// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CTransitionHUD.h"
#include "Utility/CDebug.h"
#include "Widget/World/CLoadingScreenWidget.h"

ACTransitionHUD::ACTransitionHUD()
{

}

void ACTransitionHUD::BeginPlay()
{
	if (LoadingScreenWidgetClass)
	{
		LoadingScreenWidget = CreateWidget<UCLoadingScreenWidget>(GetWorld(), LoadingScreenWidgetClass);
		LoadingScreenWidget->AddToViewport(5); //그보다 아래
		LoadingScreenWidget->SetVisibility(ESlateVisibility::Visible);
		LoadingScreenWidget->bIsFocusable = true;
	}
	CDebug::Print(TEXT("FUCK YOU"));
}
