// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CRespawnLocation.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCRespawnLocation : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
		void SetOwnerActor(class AActor* InActor);

	void SetRespawnLocationOnWorldMap(float LocationX, float LocationY);

private:
	UFUNCTION()
		void DestroyWidget(class AActor* DestroyedActor);
	UFUNCTION()
		void SetRespawnLoctionName(const FText& InText);
private:
	UPROPERTY(meta = (BindWidget))
		class UButton* RespawnLocationButton;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* RespawnLoctionName;


};
