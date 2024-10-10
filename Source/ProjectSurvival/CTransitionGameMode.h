// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CTransitionGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API ACTransitionGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACTransitionGameMode();

	// TArray<UCItemBase> InventoryItems;  // 서버에서 관리하는 공용 인벤토리

	 //// 인벤토리 최신화 함수
	 //void UpdateInventory(const TArray<UCItemBase>& NewInventory);
};
