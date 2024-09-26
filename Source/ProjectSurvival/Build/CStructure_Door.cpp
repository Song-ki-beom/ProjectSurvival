#include "Build/CStructure_Door.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Utility/CDebug.h"

ACStructure_Door::ACStructure_Door()
{

}

void ACStructure_Door::CheckCenter()
{
	FHitResult centerBoxHitResult;
	FVector centerBoxLocation = this->GetActorLocation() - FVector(0, 0, 50);
	FVector centerBoxSize = FVector(50, 5, 75);
	FRotator centerBoxOrientation;
	if (!bUp_DoorFrameHit)
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

void ACStructure_Door::CheckUp_DoorFrame()
{
	FHitResult upHitResult;
	FVector upStartLocation = this->GetActorLocation();
	FVector upEndLocation = this->GetActorLocation() + this->GetActorUpVector() * 250.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> upObjectTypeQuery;
	upObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel5));

	TArray<AActor*> upBoxActorsToIgnore;
	upBoxActorsToIgnore.Add(this);

	bUp_DoorFrameHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		upStartLocation,
		upEndLocation,
		upObjectTypeQuery,
		false,
		upBoxActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		upHitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red
	);

	if (bUp_DoorFrameHit)
	{
		CDebug::Print("Result : ", upHitResult.GetComponent());
		this->SetActorLocation(upHitResult.GetComponent()->GetComponentLocation() + upHitResult.GetComponent()->GetForwardVector() * 15.0f + upHitResult.ImpactNormal * 120.0f);
		CenterRotation = upHitResult.GetComponent()->GetComponentRotation() + FRotator(0, -90, 0);
		this->SetActorRotation(CenterRotation);
	}
}

void ACStructure_Door::OpenDoor()
{

}

void ACStructure_Door::CloseDoor()
{

}
