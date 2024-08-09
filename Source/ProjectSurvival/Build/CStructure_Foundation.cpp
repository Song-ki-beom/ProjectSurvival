#include "Build/CStructure_Foundation.h"

ACStructure_Foundation::ACStructure_Foundation()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACStructure_Foundation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}