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

	//void CreateActorInventoryComponent();

	//class UCActorInventoryComponent* GetActorInventoryComponent() { return ActorInventoryComponent; }

	void OpenActorInventory(const class ACSurvivor* Survivor, class AActor* Actor) override;

	TArray<UCItemBase*> GetSharedInventoryObject() { return SharedInventoryObject; }

protected:
	UPROPERTY(EditAnywhere)
		class UBoxComponent* DownBox;

	//UPROPERTY(VisibleAnywhere)
	//	class UCActorInventoryComponent* ActorInventoryComponent;
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


public:
	//UFUNCTION()
	//	void AddInventoryID(FName InID);
	
	void PerformAddID(FName InID);
	
	//UFUNCTION(BlueprintCallable, Category = "Inventory")
	//	TArray<FName> GetSharedInventoryID() { return SharedInventoryID; }

private:
	//UPROPERTY(Replicated)
	//	TArray<FName> SharedInventoryID;
	UPROPERTY(Replicated)
		TArray<UCItemBase*> SharedInventoryObject;
	//UPROPERTY()
	//	class UDataTable* ItemDataTable;


};
