// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CInventoryTooltip.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCInventoryTooltip : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	virtual void NativeConstruct() override;



public:
	//메뉴 모드에서 마우스에 의해 Hovering 되는(가리키는) Slot Reference 

	UPROPERTY(VisibleAnywhere)
	class UCInventoryItemSlot* InventorySlotBeingHovered;


	UPROPERTY(meta=(BindWidget))
	class UTextBlock* ItemName;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemType;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* DamageValue;


	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Usage;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ItemDescription;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MaxStackSize;//스택
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MaxStackSizeText;//스택

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* StackWeight; //무게



};
