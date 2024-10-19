// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ListView.h"
#include "CEarnInfoPanel.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCEarnInfoPanel : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void AddEarnedItemSlot(UObject* Item);
	void DeleteEarnedItemSlot(UObject* Item);
private:
	UPROPERTY(meta = (BindWidget))
		class UListView* EarnedItemList;
	UPROPERTY()
		TMap<UObject*, FTimerHandle> ItemTimers;
};
