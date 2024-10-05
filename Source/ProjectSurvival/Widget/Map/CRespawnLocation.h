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
	
protected:
	virtual void NativeDestruct() override;
	virtual bool Initialize() override;

public:
	UFUNCTION()
		void SetOwnerActor(class AActor* InActor);

	void SetRespawnLocationOnWorldMap(float LocationX, float LocationY);

	class UButton* GetRespawnButton() { return RespawnLocationButton; }

private:
	UFUNCTION()
		void DestroyWidget(class AActor* DestroyedActor);
	UFUNCTION()
		void SetRespawnLoctionName(const FText& InText);
	UFUNCTION()
		void OnClickRespawnLocationButton();

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* RespawnLocationButton;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* RespawnLoctionName;


};
