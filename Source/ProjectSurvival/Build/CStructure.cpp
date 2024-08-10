#include "Build/CStructure.h"

ACStructure::ACStructure()
{
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

