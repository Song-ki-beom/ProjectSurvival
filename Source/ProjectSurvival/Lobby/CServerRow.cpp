#include "Lobby/CServerRow.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Lobby/CLobbyWidget.h"

bool UCServerRow::Initialize()
{
	bool Sucess = Super::Initialize();

	if (!IsValid(ServerRowButton)) { UE_LOG(LogTemp, Warning, TEXT("ServerRowButton is invalid")); return false; }
	ServerRowButton->OnClicked.AddDynamic(this, &UCServerRow::SelectServer);

	if (!Sucess)
		return false;

	return true;
}

void UCServerRow::SetUpWidget(class UCLobbyWidget* InParent, uint32 InIndex)
{
	Parent = InParent;
	Index = InIndex;
}

void UCServerRow::SelectServer()
{
	Parent->SelectIndex(Index);
	UE_LOG(LogTemp, Warning, TEXT("SelectServer Called %d"), Index);
}