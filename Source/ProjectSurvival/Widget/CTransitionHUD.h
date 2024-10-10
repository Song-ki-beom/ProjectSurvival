// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CTransitionHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API ACTransitionHUD : public AHUD
{
	GENERATED_BODY()
public:
	ACTransitionHUD();

protected:
	virtual void BeginPlay() override;



public:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UCLoadingScreenWidget> LoadingScreenWidgetClass;
protected:
	UPROPERTY()
		class UCLoadingScreenWidget* LoadingScreenWidget;
};
