// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Status/CEnemyStatusBar.h"

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
	HealthBar->SetPercent(1.0f);

}

void UCEnemyStatusBar::InitializeEnemyCharacter(ACharacter* InCharacter)
{
	OwnerEnemyCharacter = InCharacter;
	if (OwnerEnemyCharacter == nullptr) return;
	StatusComponent = Cast<UCStatusComponent>(OwnerEnemyCharacter->GetComponentByClass(UCStatusComponent::StaticClass()));
	StatusComponent->OnHealthUpdated.AddDynamic(this, &UCEnemyStatusBar::UpdateHealthProgressBar);
}

void UCEnemyStatusBar::UpdateHealthProgressBar(float HealthPercentage)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(HealthPercentage);
	}
}


