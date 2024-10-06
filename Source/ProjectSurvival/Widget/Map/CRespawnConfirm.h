// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CRespawnConfirm.generated.h"

UCLASS()
class PROJECTSURVIVAL_API UCRespawnConfirm : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;

public:
	void SelectRespawnLocation(FText InText, FVector InVector);

private:
	UFUNCTION()
		void OnClickConfirmButton();
	UFUNCTION()
		void OnClickCancleButton();

private:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* RespawnLocationName;
	UPROPERTY(meta = (BindWidget))
		class UButton* ConfirmButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* CancleButton;

	FVector SelectedRespawnLocation;
};
