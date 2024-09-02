// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Interface/InteractionInterface.h"
#include "CMainHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API ACMainHUD : public AHUD
{
	GENERATED_BODY()
public:
	ACMainHUD();

	void DisplayMenu();
	void HideMenu();
	void ToggleMenu(); //Hide & Show Menu Toggle 
	void ShowInteractionWidget();
	void HideInteractionWidget();
	void UpdateInteractionWidget(const FInteractableData* InteractableData);
	void ShowSubMenu(FVector2D Position,class UCInventoryItemSlot* InSlotReference);
	void HideSubMenu();
	void ToggleMoreInfo();
	void ShowMoreInfo();
	void HideMoreInfo();


	//EditAnywhere, BlueprintReadWrite
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UCInventoryMenu> InventoryMenuClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UCInventorySubMenu> InventorySubMenuClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UCInteractionWidget> InteractionWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UCProduceWidget> ProduceWidgetClass;
	



protected:
	virtual void BeginPlay() override;


protected:
	UPROPERTY()
		class UCInventoryMenu* InventoryMenuWidget;
	UPROPERTY()
		class UCInteractionWidget* InteractionWidget;
	UPROPERTY()
		class UCInventorySubMenu* InventorySubMenuWidget;
	UPROPERTY()
		class UCProduceWidget* ProduceWidget;

public:
	bool bIsMenuVisible;


};
