#include "Build/CStructure_Stair.h"
#include "Kismet/KismetSystemLibrary.h"

ACStructure_Stair::ACStructure_Stair()
{
	UpBox = CreateDefaultSubobject<UBoxComponent>("UpBox");

	UpBox->SetupAttachment(PickupMesh);
}

void ACStructure_Stair::CheckCenter()
{
	FHitResult centerBoxHitResult;
	FVector centerBoxLocation = this->GetActorLocation();
	FVector centerBoxSize = FVector(100, 100, 100);
	FRotator centerBoxOrientation;
	if (!bDown_FoundationHit)
		centerBoxOrientation = GetOwner()->GetActorRotation();
	else
		centerBoxOrientation = CenterRotation;
	ETraceTypeQuery centerBoxTraceTypeQuery = ETraceTypeQuery::TraceTypeQuery3;
	bool bCenterBoxTraceComplex = false;
	TArray<AActor*> centerBoxActorsToIgnore;
	TArray<FHitResult> centerBoxHitResults;
	bCenterHit = UKismetSystemLibrary::BoxTraceSingle(
		GetWorld(),
		centerBoxLocation,
		centerBoxLocation,
		centerBoxSize,
		centerBoxOrientation,
		centerBoxTraceTypeQuery,
		bCenterBoxTraceComplex,
		centerBoxActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		centerBoxHitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red
	);
}

void ACStructure_Stair::CheckDown_Foundation()
{
	FHitResult downHitResult;
	FVector downStartLocation = UpBox->GetComponentLocation();
	FVector downEndLocation = UpBox->GetComponentLocation() + UpBox->GetUpVector() * -250.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> downObjectTypeQuery;
	downObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3));

	TArray<AActor*> downBoxActorsToIgnore;
	downBoxActorsToIgnore.Add(this);

	bDown_FoundationHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		downStartLocation,
		downEndLocation,
		downObjectTypeQuery,
		false,
		downBoxActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		downHitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red
	);

	if (bDown_FoundationHit)
	{
		this->SetActorLocation(downHitResult.GetComponent()->GetOwner()->GetActorLocation());
		this->AddActorLocalOffset(FVector(20, 20, 245));
		CenterRotation = downHitResult.GetComponent()->GetOwner()->GetActorRotation() + FRotator(0, 90, 0);
		this->SetActorRotation(CenterRotation);
	}
}
