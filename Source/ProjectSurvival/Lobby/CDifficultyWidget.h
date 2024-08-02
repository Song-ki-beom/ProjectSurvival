// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "CDifficultyWidget.generated.h"

USTRUCT(BlueprintType)
struct FDifficultyInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTexture2D* DifficultyTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText DifficultySummary;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText DifficultyDetail_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText DifficultyDetail_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText DifficultyDetail_3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText DifficultyDetail_4;
};

UCLASS()
class PROJECTSURVIVAL_API UCDifficultyWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
		class UImage* DifficultyImage;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* DifficultyText;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* DifficultyDesc_1;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* DifficultyDesc_2;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* DifficultyDesc_3;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* DifficultyDesc_4;
	UPROPERTY(meta = (BindWidget))
		class UButton* EasyButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* NormalButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* HardButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* ExtremeButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* StartButton;
	UFUNCTION()
		void OnEasy();
	UFUNCTION()
		void OnNormal();
	UFUNCTION()
		void OnHard();
	UFUNCTION()
		void OnExtreme();
	UFUNCTION()
		void OnStart();
public:
	


	
};
