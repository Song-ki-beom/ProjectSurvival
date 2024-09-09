#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Widget/Inventory/CItemBase.h"
#include "CGameStateBase.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	ACGameStateBase();

public:
	//UFUNCTION()
	//	void AddInventoryID(FName InID);
	//
	void PerformAddID(FName InID);
	//
	//UFUNCTION(BlueprintCallable, Category = "Inventory")
	//	TArray<FName> GetSharedInventoryID() { return SharedInventoryID; }

private:
	//UPROPERTY(Replicated)
	//	TArray<FName> SharedInventoryID;
	UPROPERTY(Replicated)
		TArray<UCItemBase*> SharedInventoryObject;
	UPROPERTY()
		class UDataTable* ItemDataTable;
};
