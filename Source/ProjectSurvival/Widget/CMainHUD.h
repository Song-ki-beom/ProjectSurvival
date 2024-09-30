// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Interface/InteractionInterface.h"
#include "Widget/Inventory/CInventoryItemSlot.h"
#include "CMainHUD.generated.h"

UENUM()
enum class EWidgetCall : uint8
{
	Survivor,
	Placeable,
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
	void SetWidgetVisibility(EWidgetCall InWidgetCall, class UUserWidget* InActorInventoryWidget = nullptr, class UUserWidget* InActorProduceWidget = nullptr, class AActor* InActor = nullptr);
	void DisplaySurvivorInventoryWidget();
	void DisplayProduceWidget(EWidgetCall InWidgetCall, class UUserWidget* InActorProduceWidget = nullptr, class AActor* InActor = nullptr);
	void DisplayActorInventory(EWidgetCall InWidgetCall, class UUserWidget* InWidget, class AActor* InActor);
	void ShowInteractionWidget();
	void HideInteractionWidget();
	void UpdateInteractionWidget(const FInteractableData* InteractableData);
	void ShowSubMenu(FVector2D Position, class UCInventoryItemSlot* InSlotReference, ERightClickStartWidget InRightClickStartWidget, bool bIsStackable);
	void HideSubMenu();
	void ToggleHiddenMenu();
	void ShowHiddenMenu();
	void HideHiddenMenu();
	void ExtraOptionButtonUp();
	void ExtraOptionButtonDown();
	void AddEarnedInfo(class UObject* EarnedItem);
	class UCProduceWidget* GetSurvivorProduceWidget() { return SurvivorProduceWidget; }
	class UCInventoryMenu* GetSurvivorInventoryWidget() { return SurvivorInventoryWidget; }
	class UCQuickSlot* GetQuickSlotWidget() { return QuickSlot; }
	class UCInventorySubMenu* GetInventorySubMenu() { return InventorySubMenuWidget; }
	class UCInteractionWidget* GetInteractionWidget() { return InteractionWidget; }
	class UCProduceWidget* GetPlaceableProduceWidget() { return PlaceableProduceWidget; }
	class UCMiniMap* GetMiniMap() { return MiniMap; }
	class UCWorldMap* GetWorldMap() { return WorldMap; }

	//EditAnywhere, BlueprintReadWrite
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UCInventoryMenu> InventoryMenuClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UCInventorySubMenu> InventorySubMenuClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UCInteractionWidget> InteractionWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UCProduceWidget> ProduceWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UCEarnInfoPanel> EarnInfoPanelClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UCQuickSlot> QuickSlotClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UCStatusPanel> StatusPanelClass;
	UPROPERTY(EditAnywhere,Category = "Widgets")
	TSubclassOf<class UCLowHealthWidget> LowHealthWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UCMiniMap> MiniMapClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UCWorldMap> WorldMapClass;

	

protected:
	UPROPERTY()
		class UCInventoryMenu* SurvivorInventoryWidget;
	UPROPERTY()
		class UCInteractionWidget* InteractionWidget;
	UPROPERTY()
		class UCInventorySubMenu* InventorySubMenuWidget;
	UPROPERTY()
		class UCProduceWidget* SurvivorProduceWidget;
	UPROPERTY()
		class UCInventoryPanel_Placeable* PlaceableInventoryWidget;
	UPROPERTY()
		class UCProduceWidget* PlaceableProduceWidget;
	UPROPERTY()
		class UCEarnInfoPanel* EarnInfoPanel;
	UPROPERTY()
		class UCQuickSlot* QuickSlot;
	UPROPERTY()
		class UCStatusPanel* StatusPanel;
	UPROPERTY()
		class UCLowHealthWidget* LowHealthWidget;
	UPROPERTY()
		class UCMiniMap* MiniMap;
	UPROPERTY()
		class UCWorldMap* WorldMap;
};
