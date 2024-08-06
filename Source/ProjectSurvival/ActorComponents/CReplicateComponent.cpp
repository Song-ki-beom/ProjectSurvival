#include "ActorComponents/CReplicateComponent.h"

UCReplicateComponent::UCReplicateComponent()
{

}

void UCReplicateComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UCReplicateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


