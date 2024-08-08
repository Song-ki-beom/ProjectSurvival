#include "ActorComponents/CBuildComponent.h"

UCBuildComponent::UCBuildComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCBuildComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UCBuildComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

