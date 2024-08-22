#include "Build/CStructure_Ceiling.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

ACStructure_Ceiling::ACStructure_Ceiling()
{
	PreviewBox = CreateDefaultSubobject<UBoxComponent>("PrivewBox");
	ForwardBox = CreateDefaultSubobject<UBoxComponent>("ForwardBox");
	BackwardBox = CreateDefaultSubobject<UBoxComponent>("BackwardBox");
	LeftBox = CreateDefaultSubobject<UBoxComponent>("LeftBox");
	RightBox = CreateDefaultSubobject<UBoxComponent>("RightBox");

	PreviewBox->SetupAttachment(StaticMesh);
	ForwardBox->SetupAttachment(StaticMesh);
	BackwardBox->SetupAttachment(StaticMesh);
	LeftBox->SetupAttachment(StaticMesh);
	RightBox->SetupAttachment(StaticMesh);
}

void ACStructure_Ceiling::CheckCenter()
{
}

void ACStructure_Ceiling::CheckDown_Forward()
{
}

void ACStructure_Ceiling::CheckDown_Backward()
{
}

void ACStructure_Ceiling::CheckDown_Left()
{
}

void ACStructure_Ceiling::CheckDown_Right()
{
	FHitResult down_RightHitResult;
	FVector down_RightStartLocation = RightBox->GetComponentLocation();
	FVector down_RightEndLocation = RightBox->GetComponentLocation() + RightBox->GetUpVector() * -250.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> down_RightObjectTypeQuery;
	down_RightObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel4));

	TArray<AActor*> down_RightBoxActorsToIgnore;
	down_RightBoxActorsToIgnore.Add(this);

	bDown_RightHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		down_RightStartLocation,
		down_RightEndLocation,
		down_RightObjectTypeQuery,
		false,
		down_RightBoxActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		down_RightHitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red
	);

	if (bDown_RightHit)
	{
		DrawDebugLine(GetWorld(), down_RightHitResult.GetComponent()->GetComponentLocation(), down_RightHitResult.GetComponent()->GetComponentLocation() + down_RightHitResult.ImpactNormal * 300.0f, FColor::Blue);
		this->SetActorLocation(down_RightHitResult.GetComponent()->GetComponentLocation() + down_RightHitResult.GetComponent()->GetForwardVector() * -130.0f + down_RightHitResult.ImpactNormal * 20.0f);
		CenterRotation = down_RightHitResult.GetComponent()->GetComponentRotation() + FRotator(0, -90, 0);
		this->SetActorRotation(CenterRotation);
	}
}
