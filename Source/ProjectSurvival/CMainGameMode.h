#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
//#include "Struct/CItemDataStructures.h"
#include "CMainGameMode.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACMainGameMode();

   // TArray<UCItemBase> InventoryItems;  // 서버에서 관리하는 공용 인벤토리

    //// 인벤토리 최신화 함수
    //void UpdateInventory(const TArray<UCItemBase>& NewInventory);
   
};




