// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CRespawnLocationRegist.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCRespawnLocationRegist : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;

public:
	void SetTargetActor(class ACStructure_Placeable* InPlaceable) { TargetActor = InPlaceable; }

private:
	UFUNCTION()
		void OnClickConfirmButton();
	UFUNCTION()
		void OnClickCancleButton();

private:
	UPROPERTY()
		class ACStructure_Placeable* TargetActor;

	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* RespawnLocationTextBox;
	UPROPERTY(meta = (BindWidget))
		class UButton* ConfirmButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* CancleButton;

	FText SavedLocationText;
};
