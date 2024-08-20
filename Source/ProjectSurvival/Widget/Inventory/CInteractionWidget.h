// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CInteractionWidget.generated.h"


UCLASS()
class PROJECTSURVIVAL_API UCInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateWidget(const struct FInteractableData* InteractableData);

protected:
	//InteractionProgressBar 정보 갱신
	UFUNCTION(Category = "Interaction widget | Interactable Data")
	float UpdateInteractionProgress();

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

public:
	UPROPERTY(VisibleAnywhere, Category = "Interaction widget | Player Reference")
		class ACSurvivor* PlayerReference;

protected:

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction widget | Interactable Data")
	class UTextBlock* NameText;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction widget | Interactable Data")
	class UTextBlock* ActionText;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction widget | Interactable Data")
	class UTextBlock* QuantityText;

	//상호작용에 쓰이는 변수들 
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction widget | Interactable Data")
	class UTextBlock* KeyPressText;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction widget | Interactable Data")
	class UProgressBar* InteractionProgressBar;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction widget | Interactable Data")
	float CurrentInteractionDuration;



	

};
