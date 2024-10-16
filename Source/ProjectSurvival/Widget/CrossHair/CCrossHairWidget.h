// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CCrossHairWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCCrossHairWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
		class UImage* CrossHair;
	

};
