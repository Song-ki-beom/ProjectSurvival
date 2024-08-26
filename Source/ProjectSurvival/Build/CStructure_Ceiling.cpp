#include "Build/CStructure_Ceiling.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

ACStructure_Ceiling::ACStructure_Ceiling()
{
	ForwardBox = CreateDefaultSubobject<UBoxComponent>("ForwardBox");
	BackwardBox = CreateDefaultSubobject<UBoxComponent>("BackwardBox");
	LeftBox = CreateDefaultSubobject<UBoxComponent>("LeftBox");
	RightBox = CreateDefaultSubobject<UBoxComponent>("RightBox");

	ForwardBox->SetupAttachment(StaticMesh);
	BackwardBox->SetupAttachment(StaticMesh);
	LeftBox->SetupAttachment(StaticMesh);
	RightBox->SetupAttachment(StaticMesh);
}

void ACStructure_Ceiling::CheckCenter()
{
	FHitResult centerBoxHitResult;
	FVector centerBoxLocation = this->GetActorLocation();
	FVector centerBoxSize = FVector(135, 135, 10);
	FRotator centerBoxOrientation;
	if (!bRightHit && !bLeftHit && !bBackwardHit && !bForwardHit)
		centerBoxOrientation = GetOwner()->GetActorRotation();
	else
		centerBoxOrientation = CenterRotation;
	ETraceTypeQuery centerBoxTraceTypeQuery = ETraceTypeQuery::TraceTypeQuery2;
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

void ACStructure_Ceiling::CheckForward()
{
	FHitResult forwardHitResult;
	FVector forwardStartLocation = this->GetActorLocation();
	FVector forwardEndLocation = this->GetActorLocation() + this->GetActorForwardVector() * -200.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> forwardObjectTypeQuery;
	forwardObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel4));

	TArray<AActor*> forwardBoxActorsToIgnore;
	forwardBoxActorsToIgnore.Add(this);

	bForwardHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		forwardStartLocation,
		forwardEndLocation,
		forwardObjectTypeQuery,
		false,
		forwardBoxActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		forwardHitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red
	);

	if (bForwardHit)
	{
		//DrawDebugLine(GetWorld(), forwardHitResult.GetComponent()->GetComponentLocation(), forwardHitResult.GetComponent()->GetComponentLocation() + forwardHitResult.ImpactNormal * 300.0f, FColor::Blue);
		this->SetActorLocation(forwardHitResult.GetComponent()->GetComponentLocation() + forwardHitResult.ImpactNormal * 130.0f + forwardHitResult.GetComponent()->GetUpVector() * 20.0f);
		CenterRotation = forwardHitResult.GetComponent()->GetComponentRotation() + FRotator(0, 180, 0);
		this->SetActorRotation(CenterRotation);
	}
}

void ACStructure_Ceiling::CheckBackward()
{
	FHitResult backwardHitResult;
	FVector backwardStartLocation = this->GetActorLocation();
	FVector backwardEndLocation = this->GetActorLocation() + this->GetActorForwardVector() * 200.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> backwardObjectTypeQuery;
	backwardObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel4));

	TArray<AActor*> backwardBoxActorsToIgnore;
	backwardBoxActorsToIgnore.Add(this);

	bBackwardHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		backwardStartLocation,
		backwardEndLocation,
		backwardObjectTypeQuery,
		false,
		backwardBoxActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		backwardHitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red
	);

	if (bBackwardHit)
	{
		//DrawDebugLine(GetWorld(), backwardHitResult.GetComponent()->GetComponentLocation(), backwardHitResult.GetComponent()->GetComponentLocation() + backwardHitResult.ImpactNormal * 300.0f, FColor::Blue);
		this->SetActorLocation(backwardHitResult.GetComponent()->GetComponentLocation() + backwardHitResult.ImpactNormal * 130.0f + backwardHitResult.GetComponent()->GetUpVector() * 20.0f);
		CenterRotation = backwardHitResult.GetComponent()->GetComponentRotation() + FRotator(0, 0, 0);
		this->SetActorRotation(CenterRotation);
	}
}

void ACStructure_Ceiling::CheckLeft()
{
	FHitResult leftHitResult;
	FVector leftStartLocation = this->GetActorLocation();
	FVector leftEndLocation = this->GetActorLocation() + this->GetActorRightVector() * -200.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> leftObjectTypeQuery;
	leftObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel4));

	TArray<AActor*> leftBoxActorsToIgnore;
	leftBoxActorsToIgnore.Add(this);

	bLeftHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		leftStartLocation,
		leftEndLocation,
		leftObjectTypeQuery,
		false,
		leftBoxActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		leftHitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red
	);

	if (bLeftHit)
	{
		//DrawDebugLine(GetWorld(), leftHitResult.GetComponent()->GetComponentLocation(), leftHitResult.GetComponent()->GetComponentLocation() + leftHitResult.ImpactNormal * 300.0f, FColor::Blue);
		this->SetActorLocation(leftHitResult.GetComponent()->GetComponentLocation() + leftHitResult.ImpactNormal * 130.0f + leftHitResult.GetComponent()->GetUpVector() * 20.0f);
		CenterRotation = leftHitResult.GetComponent()->GetComponentRotation() + FRotator(0, 90, 0);
		this->SetActorRotation(CenterRotation);
	}
}

void ACStructure_Ceiling::CheckRight()
{
	FHitResult rightHitResult;
	FVector rightStartLocation = this->GetActorLocation();
	FVector rightEndLocation = this->GetActorLocation() + this->GetActorRightVector() * 200.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> rightObjectTypeQuery;
	rightObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel4));

	TArray<AActor*> rightBoxActorsToIgnore;
	rightBoxActorsToIgnore.Add(this);

	bRightHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		rightStartLocation,
		rightEndLocation,
		rightObjectTypeQuery,
		false,
		rightBoxActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		rightHitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red
	);

	if (bRightHit)
	{
		//DrawDebugLine(GetWorld(), rightHitResult.GetComponent()->GetComponentLocation(), rightHitResult.GetComponent()->GetComponentLocation() + rightHitResult.ImpactNormal * 300.0f, FColor::Blue);
		this->SetActorLocation(rightHitResult.GetComponent()->GetComponentLocation() + rightHitResult.ImpactNormal * 130.0f + rightHitResult.GetComponent()->GetUpVector() * 20.0f);
		CenterRotation = rightHitResult.GetComponent()->GetComponentRotation() + FRotator(0, -90, 0);
		this->SetActorRotation(CenterRotation);
	}
}