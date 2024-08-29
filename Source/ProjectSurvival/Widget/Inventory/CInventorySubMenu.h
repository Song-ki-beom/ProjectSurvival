// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CInventorySubMenu.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnFocusOnSubMenuEnded);

UCLASS()
class PROJECTSURVIVAL_API UCInventorySubMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;

public:
	FOnFocusOnSubMenuEnded OnFocusOnSubMenuEnded;
};
