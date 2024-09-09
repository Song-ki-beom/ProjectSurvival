// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "CEarnInfoSlot.generated.h"

UCLASS()
class PROJECTSURVIVAL_API UCEarnInfoSlot : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	

public:
	virtual void NativeConstruct() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;


private:
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Item")
		class UImage* ItemIcon;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Item")
		class UTextBlock* ItemName;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Item")
		class UTextBlock* QuantityText;

};
