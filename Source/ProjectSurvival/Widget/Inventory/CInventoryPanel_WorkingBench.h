// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CInventoryPanel_WorkingBench.generated.h"

UCLASS()
class PROJECTSURVIVAL_API UCInventoryPanel_WorkingBench : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:
	UFUNCTION()
		void ChangeButtonColor();

private:
	UPROPERTY(meta = (BindWidget))
		class UWrapBox* WorkingBenchInventoryPanel;
	UPROPERTY(meta = (BindWidget))
		class UButton* TestButton;
	
};
