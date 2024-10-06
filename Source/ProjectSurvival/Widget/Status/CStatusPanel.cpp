// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Status/CStatusPanel.h"
#include "ActorComponents/CStatusComponent.h"
#include "ActorComponents/CInventoryComponent.h"
#include "GameFramework/Character.h"
#include "Components/ProgressBar.h"
void UCStatusPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	
}

void UCStatusPanel::NativeConstruct()
{
	Super::NativeConstruct();
	
	ACharacter* OwnerCharacter = GetOwningPlayer()->GetCharacter();
	if (OwnerCharacter == nullptr) return;
	StatusComponent = Cast<UCStatusComponent>(OwnerCharacter->GetComponentByClass(UCStatusComponent::StaticClass()));
	StatusComponent->OnHealthUpdated.AddDynamic(this, &UCStatusPanel::UpdateHealthProgressBar);
	StatusComponent->OnHungerUpdated.AddDynamic(this, &UCStatusPanel::UpdateHungerProgressBar);
	StatusComponent->OnStaminaUpdated.AddDynamic(this, &UCStatusPanel::UpdateStaminaProgressBar);
	
	InventoryComponent = Cast<UCInventoryComponent>(OwnerCharacter->GetComponentByClass(UCInventoryComponent::StaticClass()));
	InventoryComponent->OnInventoryUpdated.AddUObject(this, &UCStatusPanel::UpdateWeightProgressBar);

	WeightBar->SetPercent(0.0f);

}

void UCStatusPanel::UpdateHealthProgressBar(float HealthPercentage)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(HealthPercentage);
	}

}

void UCStatusPanel::UpdateHungerProgressBar(float HungerPercentage)
{
	if (HungerBar)
	{
		HungerBar->SetPercent(HungerPercentage);
	}

}

void UCStatusPanel::UpdateStaminaProgressBar(float StaminaPercentage)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(StaminaPercentage);
	}

}

void UCStatusPanel::UpdateWeightProgressBar()
{
	if (WeightBar && InventoryComponent)
	{
		float Percentage = InventoryComponent->GetInventoryWeightRatio();
		WeightBar->SetPercent(Percentage);
	}
}


