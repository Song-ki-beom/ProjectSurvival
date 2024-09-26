#include "Build/CStructure_DoorFrame.h"
#include "Kismet/KismetSystemLibrary.h"

ACStructure_DoorFrame::ACStructure_DoorFrame()
{
	UpBox = CreateDefaultSubobject<UBoxComponent>("UpBox");

	UpBox->SetupAttachment(PickupMesh);
}

void ACStructure_DoorFrame::CheckCenter()
{
	FHitResult centerBoxHitResult;
	FVector centerBoxLocation = this->GetActorLocation();
	FVector centerBoxSize = FVector(135, 10, 135);
	FRotator centerBoxOrientation;
	if (!bDown_FoundationHit && !bDown_CeilingHit)
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

void ACStructure_DoorFrame::CheckDown_Foundation()
{
	FHitResult downHitResult;
	FVector downStartLocation = this->GetActorLocation();
	FVector downEndLocation = this->GetActorLocation() + this->GetActorUpVector() * -250.0f;
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
		//DrawDebugLine(GetWorld(), downHitResult.GetComponent()->GetComponentLocation(), downHitResult.GetComponent()->GetComponentLocation() + downHitResult.ImpactNormal * 300.0f, FColor::Blue);
		this->SetActorLocation(downHitResult.GetComponent()->GetComponentLocation() + downHitResult.GetComponent()->GetForwardVector() * 15.0f + downHitResult.ImpactNormal * 225.0f);
		CenterRotation = downHitResult.GetComponent()->GetComponentRotation() + FRotator(0, -90, 0);
		this->SetActorRotation(CenterRotation);
	}
}

void ACStructure_DoorFrame::CheckDown_Ceiling()
{
	FHitResult downHitResult;
	FVector downStartLocation = this->GetActorLocation();
	FVector downEndLocation = this->GetActorLocation() + this->GetActorUpVector() * -250.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> downObjectTypeQuery;
	downObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel6));

	TArray<AActor*> downBoxActorsToIgnore;
	downBoxActorsToIgnore.Add(this);

	bDown_CeilingHit = UKismetSystemLibrary::LineTraceSingleForObjects(
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

	if (bDown_CeilingHit)
	{
		this->SetActorLocation(downHitResult.GetComponent()->GetComponentLocation() + downHitResult.GetComponent()->GetForwardVector() * 15.0f + downHitResult.ImpactNormal * 170.0f);
		CenterRotation = downHitResult.GetComponent()->GetComponentRotation() + FRotator(0, -90, 0);
		this->SetActorRotation(CenterRotation);
	}
}
