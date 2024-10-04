// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CEnemyStatusBar.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCEnemyStatusBar : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	void InitializeEnemyCharacter(ACharacter* InCharacter);
	void UpdateHealthColor(FLinearColor InColor);

private:
	UFUNCTION()
		void UpdateHealthProgressBar(float HealthPercentage);

private:
	class ACharacter*  OwnerEnemyCharacter;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HealthBar;
private:
	class UCStatusComponent* StatusComponent;

};
