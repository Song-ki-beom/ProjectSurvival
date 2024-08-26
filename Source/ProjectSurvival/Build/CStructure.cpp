#include "Build/CStructure.h"
#include "Components/BoxComponent.h"

ACStructure::ACStructure()
{
	bReplicates = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
}

void ACStructure::BeginPlay()
{
	Super::BeginPlay();
	SaveOriginMaterial();
}

void ACStructure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACStructure::SaveOriginMaterial()
{
	if (IsValid(StaticMesh->GetStaticMesh()))
		OriginMaterial = StaticMesh->GetStaticMesh()->GetMaterial(0);
}

