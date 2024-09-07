#include "ActorComponents/CActorInventoryComponent.h"
#include "Widget/Inventory/CInventoryPanel_WorkingBench.h"
#include "Widget/Inventory/CItemBase.h"
#include "Net/UnrealNetwork.h"
#include "World/CPickup.h"
#include "Utility/CDebug.h"

UCActorInventoryComponent::UCActorInventoryComponent()
{
}

void UCActorInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerActor = Cast<AActor>(this->GetOwner());
}