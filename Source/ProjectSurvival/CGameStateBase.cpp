#include "CGameStateBase.h"
#include "Blueprint/UserWidget.h"
#include "Widget/Chatting/CChattingBox.h"
#include "Framework/Application/NavigationConfig.h"
#include "Utility/CDebug.h"

void ACGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	FSlateApplication::Get().GetNavigationConfig()->bTabNavigation = false;
}