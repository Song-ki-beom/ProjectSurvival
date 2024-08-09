#include "Build/CStructure.h"

ACStructure::ACStructure()
{
	ConstructorHelpers::FObjectFinder<UMaterialInstance> redMaterialFinder(TEXT("MaterialInstanceConstant'/Game/PirateIsland/Include/Materials/Builds/MI_Build_Red.MI_Build_Red'"));
	if (redMaterialFinder.Succeeded())
	{
		RedMaterial = redMaterialFinder.Object;
	}

	ConstructorHelpers::FObjectFinder<UMaterialInstance> greenMaterialFinder(TEXT("MaterialInstanceConstant'/Game/PirateIsland/Include/Materials/Builds/MI_Build_Green.MI_Build_Green'"));
	if (greenMaterialFinder.Succeeded())
	{
		GreenMaterial = greenMaterialFinder.Object;
	}

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
}

void ACStructure::BeginPlay()
{
	Super::BeginPlay();
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

