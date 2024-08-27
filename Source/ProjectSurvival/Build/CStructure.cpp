#include "Build/CStructure.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

ACStructure::ACStructure()
{
	bReplicates = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	PreviewBox = CreateDefaultSubobject<UBoxComponent>("PrivewBox");
	PreviewBox->SetupAttachment(StaticMesh);
	PreviewBox->SetIsReplicated(true);
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


void ACStructure::BeginFocus()
{
	Super::BeginFocus();
}

void ACStructure::EndFocus()
{
	Super::EndFocus();
}

void ACStructure::BeginInteract()
{
	Super::BeginInteract();
}

void ACStructure::EndInteract()
{
	Super::EndInteract();
}

void ACStructure::Interact(ACSurvivor* PlayerCharacter)
{
	Super::Interact(PlayerCharacter);
}

void ACStructure::BroadcastDestroyPreviewBox_Implementation()
{
	PreviewBox->DestroyComponent();
}

void ACStructure::SaveOriginMaterial()
{
	if (IsValid(StaticMesh->GetStaticMesh()))
		OriginMaterial = StaticMesh->GetStaticMesh()->GetMaterial(0);
}

