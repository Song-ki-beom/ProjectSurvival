// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Status/CEnemyStatusBar.h"
#include "Components/Image.h"
#include "ActorComponents/CStatusComponent.h"
#include "ActorComponents/CInventoryComponent.h"
#include "GameFramework/Character.h"
#include "Components/ProgressBar.h"
#include "Utility/CDebug.h"

void UCEnemyStatusBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();


}

void UCEnemyStatusBar::NativeConstruct()
{
	Super::NativeConstruct();	
	StartBlinkAnimation();
	HealthBar->SetPercent(1.0f);
	StarvationIcon->SetVisibility(ESlateVisibility::Collapsed);
	ExhaustIcon->SetVisibility(ESlateVisibility::Collapsed);
	FriendShipBar->SetVisibility(ESlateVisibility::Hidden);
}

void UCEnemyStatusBar::InitializeEnemyCharacter(ACharacter* InCharacter)
{
	OwnerEnemyCharacter = InCharacter;
	if (OwnerEnemyCharacter == nullptr) return;
	StatusComponent = Cast<UCStatusComponent>(OwnerEnemyCharacter->GetComponentByClass(UCStatusComponent::StaticClass()));
	StatusComponent->OnHealthUpdated.AddDynamic(this, &UCEnemyStatusBar::UpdateHealthProgressBar);
	StatusComponent->OnExhaustDetected.AddDynamic(this, &UCEnemyStatusBar::ToggleExhaustIcon);
	StatusComponent->OnStarvationDetected.AddDynamic(this, &UCEnemyStatusBar::ToggleStarvationIcon);
	StatusComponent->OnExhaustDetected.AddDynamic(this, &UCEnemyStatusBar::ToggleFriendShipBar);
	StatusComponent->OnFriendShipUpdated.AddDynamic(this, &UCEnemyStatusBar::UpdateFriendShipProgressBar);
	StatusComponent->OnBecameFriendly.AddDynamic(this, &UCEnemyStatusBar::OnBecameFriendlyHandler);
}

void UCEnemyStatusBar::UpdateHealthColor(FLinearColor InColor)
{
	if (HealthBar)
	{
		HealthBar->SetFillColorAndOpacity(InColor);
	}
}


void UCEnemyStatusBar::ToggleStarvationIcon(bool bSetVisible)
{
	if (bSetVisible)
	{
		
		StarvationIcon->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		StarvationIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UCEnemyStatusBar::ToggleFriendShipBar(bool bSetVisible)
{
	if (bSetVisible)
	{

		FriendShipBar->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		FriendShipBar->SetVisibility(ESlateVisibility::Hidden);
	}
}


void UCEnemyStatusBar::ToggleExhaustIcon(bool bSetVisible)
{
	if(bSetVisible)
	{
		if (StarvationIcon->IsVisible())
		{
			StarvationIcon->SetVisibility(ESlateVisibility::Collapsed);
		}
		ExhaustIcon->SetVisibility(ESlateVisibility::Visible);
	}
	else 
	{
		PlayAnimation(BlinkAnim);
		ExhaustIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UCEnemyStatusBar::UpdateHealthProgressBar(float HealthPercentage)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(HealthPercentage);
	}
}

void UCEnemyStatusBar::UpdateFriendShipProgressBar(float FriendShipPercentage)
{
	if (FriendShipBar)
	{
		FriendShipBar->SetPercent(FriendShipPercentage);
	}
}

void UCEnemyStatusBar::OnBecameFriendlyHandler()
{
	StarvationIcon->SetVisibility(ESlateVisibility::Collapsed);
	ExhaustIcon->SetVisibility(ESlateVisibility::Collapsed);
	FriendShipBar->SetVisibility(ESlateVisibility::Hidden);
}

void UCEnemyStatusBar::StartBlinkAnimation()
{
	if (BlinkAnim)
	{
		BlinkWidgetAnimationEvent.Clear();
		BlinkWidgetAnimationEvent.BindUFunction(this, FName(FString(TEXT("OnBlinkAnimationFinished"))));
		BindToAnimationFinished(BlinkAnim, BlinkWidgetAnimationEvent);
		PlayAnimation(BlinkAnim);
	}
}

void UCEnemyStatusBar::OnBlinkAnimationFinished()
{
	PlayAnimation(BlinkAnim);
}


