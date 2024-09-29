// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Status/CStatusPanel.h"
#include "ActorComponents/CStatusComponent.h"
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

}

void UCStatusPanel::UpdateHealthProgressBar(float HealthPercentage)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(HealthPercentage);
	}

}

void UCStatusPanel::UpdateHungerProgressBar(float StaminaPercentage)
{
	if (HungerBar)
	{
		HungerBar->SetPercent(StaminaPercentage);
	}

}
