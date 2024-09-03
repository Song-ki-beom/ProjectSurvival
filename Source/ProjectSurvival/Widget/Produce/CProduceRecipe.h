// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CProduceRecipe.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCProduceRecipe : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetResourceIcon(UTexture2D* InTexture2D);
	void SetResourceName(FText InText);
	void SetResourceQuantity(int32 InventoryQuantity, int32 DemandQuantity);

private:
	UPROPERTY(meta = (BindWidget))
		class UImage* ResourceIcon;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ResourceName;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ResourceQuantity;
};
