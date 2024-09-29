// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CStatusPanel.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCStatusPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
		void UpdateHealthProgressBar(float HealthPercentage);
	UFUNCTION()
		void UpdateHungerProgressBar(float StaminaPercentage);


private:
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HungerBar;

private:
	class UCStatusComponent* StatusComponent;


};
