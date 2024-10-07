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


	UFUNCTION()
	void ToggleStarvationIcon(bool bSetVisible);
	UFUNCTION()
	void ToggleExhaustIcon(bool bSetVisible);
	UFUNCTION()
	void ToggleFriendShipBar(bool bSetVisible);
	UFUNCTION()
		void UpdateHealthColor(FLinearColor InColor);

	
	UFUNCTION()
	void StartBlinkAnimation();
	UFUNCTION()
	void OnBlinkAnimationFinished();


private:
	UFUNCTION()
		void UpdateFriendShipProgressBar(float FriendShipPercentage);
	UFUNCTION()
		void UpdateHealthProgressBar(float HealthPercentage);
	UFUNCTION()
		void OnBecameFriendlyHandler();

private:
	class ACharacter*  OwnerEnemyCharacter;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* FriendShipBar;
	UPROPERTY(meta = (BindWidget))
		class UImage* StarvationIcon;
	UPROPERTY(meta = (BindWidget))
		class UImage* ExhaustIcon;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* BlinkAnim;
	FWidgetAnimationDynamicEvent BlinkWidgetAnimationEvent;



private:
	class UCStatusComponent* StatusComponent;

};
