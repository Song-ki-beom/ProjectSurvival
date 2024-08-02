#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CSurvivorName.generated.h"

/**
* 이 클래스는 캐릭터에 부착되어 관리되는 클래스이다.
* 머리 위에 이름을 표시하는 목적의 위젯이며, 해당 소유 캐릭터에서 관리됨
*/

UCLASS()
class PROJECTSURVIVAL_API UCSurvivorName : public UUserWidget
{
	GENERATED_BODY()
	
};
