#include "ActorComponents/CActorInventoryComponent.h"
#include "Widget/Inventory/CInventoryPanel_WorkingBench.h"
#include "Widget/Inventory/CItemBase.h"
#include "Net/UnrealNetwork.h"
#include "World/CPickup.h"
#include "Character/CSurvivor.h"
#include "Utility/CDebug.h"

UCActorInventoryComponent::UCActorInventoryComponent()
{
//	SetIsReplicatedByDefault(true);
}

void UCActorInventoryComponent::BeginPlay()
{
//	Super::BeginPlay();
//
//	OwnerActor = Cast<AActor>(this->GetOwner());
//	if (OwnerActor)
//		CDebug::Print("Component Owner : ", OwnerActor, FColor::Blue);
}

//void UCActorInventoryComponent::PerformAddItem(UCItemBase* InItem, const int32 AmountToAdd)
//{
//	CDebug::Print("PerformAddItem");
//}
//
//void UCActorInventoryComponent::RequestAddItem_Implementation(class UCItemBase* InItem, const int32 AmountToAdd)
//{
//	PerformAddItem(InItem, AmountToAdd);
//}