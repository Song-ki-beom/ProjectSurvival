// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Interface/InteractionInterface.h"
#include "CMainHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API ACMainHUD : public AHUD
{
	GENERATED_BODY()
public:
	ACMainHUD();

	void DisplayMenu();
	void HideMenu();

	void ShowInteractionWidget();
	void HideInteractionWidget();
	void UpdateInteractionWidget(const FInteractableData* InteractableData);
	//EditAnywhere, BlueprintReadWrite
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UCMainMenu> MainMenuClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UCInteractionWidget> InteractionWidgetClass;


protected:
	virtual void BeginPlay() override;


protected:
	UPROPERTY()
		class UCMainMenu* MainMenuWidget;
	UPROPERTY()
		class UCInteractionWidget* InteractionWidget;


public:
	bool bIsMenuVisible;


};
