// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CDragItemVisual.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCDragItemVisual : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	//VisibleAnywhere
UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget))
	class UBorder* ItemBorder; //ItemSlot 의 외부 프레임 

UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget))
	class UImage* ItemIcon; //아이템을 나타내는 Icon

UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget))
	class UTextBlock* ItemQuantity; // 수량 

};
