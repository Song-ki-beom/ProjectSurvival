#pragma once

#include "CoreMinimal.h"
#include "Build/CStructure.h"
#include "Widget/CMainHUD.h"
#include "CStructure_Placeable.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACStructure_Placeable : public ACStructure
{
	GENERATED_BODY()
	
public:
	ACStructure_Placeable();

protected:
	virtual void BeginPlay() override;

public:
	void CheckDown_FoundationAndCeiling();
	bool GetPlaceableDown_FoundationAndCeilingHit() { return bDown_FoundationAndCeilingActorHit; }

	void CheckCenter();
	bool GetPlaceableCenterHit() { return bCenterHit; }

	float GetPlaceableHeight() { return PlaceableHeight; }

	void OpenActorInventory(const class ACSurvivor* Survivor) override;

protected:
	UPROPERTY(EditAnywhere)
		class UBoxComponent* DownBox;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> ActorInventoryWidgetClass;
	UPROPERTY(EditAnywhere)
		EWidgetCall WidgetCaller;
	UPROPERTY()
		class UUserWidget* ActorInventoryWidget;

	bool bDown_FoundationAndCeilingActorHit;
	bool bCenterHit;
	float PlaceableHeight;

	FRotator CenterRotation;
};
