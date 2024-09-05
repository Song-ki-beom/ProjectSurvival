// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CInventoryMenu.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnMainMenuToggled);


UCLASS()
class PROJECTSURVIVAL_API UCInventoryMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	//NativeOnDrop <<- UMG (Unreal Motion Graphics) 시스템에서 드래그 앤 드롭 기능을 구현할 때 사용되는 메서드, UI 밖으로 Drop 이 감지되면 해당 함수 호출 
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	UFUNCTION(Server, Reliable)
		void RequestDropItem(class UCItemBase* ItemToDrop, const int32 QuantityToDrop);
	UFUNCTION(NetMulticast, Reliable)
		void BroadCastDropItem(class UCItemBase* ItemToDrop, const int32 QuantityToDrop);
	


public:
	UPROPERTY()
		class ACSurvivor* PlayerCharacter;

	FOnMainMenuToggled OnMainMenuToggled;

};
