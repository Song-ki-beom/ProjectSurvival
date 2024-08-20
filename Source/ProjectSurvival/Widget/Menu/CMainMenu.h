// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CMainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCMainMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	
	//NativeOnDrop <<- UMG (Unreal Motion Graphics) 시스템에서 드래그 앤 드롭 기능을 구현할 때 사용되는 메서드
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;


public:
	UPROPERTY()
		class ACSurvivor* PlayerCharacter;



};
