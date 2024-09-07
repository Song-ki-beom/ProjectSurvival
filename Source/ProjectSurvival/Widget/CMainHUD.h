// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Interface/InteractionInterface.h"
#include "CMainHUD.generated.h"

UENUM()
enum class EWidgetCall : uint8
{
	Survivor,
	WorkBench,
	CloseWidget
};

UCLASS()
class PROJECTSURVIVAL_API ACMainHUD : public AHUD
{
	GENERATED_BODY()
public:
	ACMainHUD();

protected:
	virtual void BeginPlay() override;

public:
	void SetWidgetVisibility(EWidgetCall InWidgetCall, class UUserWidget* InWidget = nullptr);
	void DisplaySurvivorInventoryWidget();
	void DisplayProduceWidget(EWidgetCall InWidgetCall);
	void DisplayActorInventory(EWidgetCall InWidgetCall, class UUserWidget* InWidget);
	void ShowInteractionWidget();
	void HideInteractionWidget();
	void UpdateInteractionWidget(const FInteractableData* InteractableData);
	void ShowSubMenu(FVector2D Position,class UCInventoryItemSlot* InSlotReference);
	void HideSubMenu();
	void ToggleHiddenMenu();
	void ShowHiddenMenu();
	void HideHiddenMenu();
	void ExtraOptionButtonUp();
	void ExtraOptionButtonDown();
	class UCProduceWidget* GetProduceWidget() { return ProduceWidget; }

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
	UPROPERTY()
		class UCInventoryMenu* SurvivorInventoryWidget;
	UPROPERTY()
		class UCInteractionWidget* InteractionWidget;
	UPROPERTY()
		class UCInventorySubMenu* InventorySubMenuWidget;
	UPROPERTY()
		class UCProduceWidget* ProduceWidget;
	UPROPERTY()
		class UUserWidget* ActorInventoryWidget;

public:
	//bool bIsMenuVisible;
	

};
