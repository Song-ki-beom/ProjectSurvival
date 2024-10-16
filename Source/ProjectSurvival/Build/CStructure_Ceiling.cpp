#include "Build/CStructure_Ceiling.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "Utility/CDebug.h"

ACStructure_Ceiling::ACStructure_Ceiling()
{
	ForwardBox = CreateDefaultSubobject<UBoxComponent>("ForwardBox");
	BackwardBox = CreateDefaultSubobject<UBoxComponent>("BackwardBox");
	LeftBox = CreateDefaultSubobject<UBoxComponent>("LeftBox");
	RightBox = CreateDefaultSubobject<UBoxComponent>("RightBox");

	ForwardBox->SetupAttachment(PickupMesh);
	BackwardBox->SetupAttachment(PickupMesh);
	LeftBox->SetupAttachment(PickupMesh);
	RightBox->SetupAttachment(PickupMesh);
}

void ACStructure_Ceiling::CheckCenter()
{
	FHitResult centerBoxHitResult;
	FVector centerBoxLocation = this->GetActorLocation();
	FVector centerBoxSize = FVector(120, 120, 10);
	FRotator centerBoxOrientation;
	if (!bRight_WallHit && !bLeft_WallHit && !bBackward_WallHit && !bForward_WallHit && !bRight_CeilingHit && !bLeft_CeilingHit && !bBackward_CeilingHit && !bForward_CeilingHit)
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
		EDrawDebugTrace::None,
		centerBoxHitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red
	);
}

void ACStructure_Ceiling::CheckForward_Wall()
{
	FHitResult forwardHitResult;
	FVector forwardStartLocation = this->GetActorLocation();
	FVector forwardEndLocation = this->GetActorLocation() + this->GetActorForwardVector() * -200.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> forwardObjectTypeQuery;
	forwardObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel4));
	forwardObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel5));

	TArray<AActor*> forwardBoxActorsToIgnore;
	forwardBoxActorsToIgnore.Add(this);

	bForward_WallHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		forwardStartLocation,
		forwardEndLocation,
		forwardObjectTypeQuery,
		false,
		forwardBoxActorsToIgnore,
		EDrawDebugTrace::None,
		forwardHitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red
	);

	if (bForward_WallHit)
	{
		CDebug::Print("Hi");
		//DrawDebugLine(GetWorld(), forwardHitResult.GetComponent()->GetComponentLocation(), forwardHitResult.GetComponent()->GetComponentLocation() + forwardHitResult.ImpactNormal * 300.0f, FColor::Blue);
		this->SetActorLocation(forwardHitResult.GetComponent()->GetComponentLocation() + forwardHitResult.ImpactNormal * 130.0f + forwardHitResult.GetComponent()->GetUpVector() * 20.0f);
		CenterRotation = forwardHitResult.GetComponent()->GetComponentRotation() + FRotator(0, 180, 0);
		this->SetActorRotation(CenterRotation);
	}
}

void ACStructure_Ceiling::CheckBackward_Wall()
{
	FHitResult backwardHitResult;
	FVector backwardStartLocation = this->GetActorLocation();
	FVector backwardEndLocation = this->GetActorLocation() + this->GetActorForwardVector() * 200.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> backwardObjectTypeQuery;
	backwardObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel4));
	backwardObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel5));

	TArray<AActor*> backwardBoxActorsToIgnore;
	backwardBoxActorsToIgnore.Add(this);

	bBackward_WallHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		backwardStartLocation,
		backwardEndLocation,
		backwardObjectTypeQuery,
		false,
		backwardBoxActorsToIgnore,
		EDrawDebugTrace::None,
		backwardHitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red
	);

	if (bBackward_WallHit)
	{
		//DrawDebugLine(GetWorld(), backwardHitResult.GetComponent()->GetComponentLocation(), backwardHitResult.GetComponent()->GetComponentLocation() + backwardHitResult.ImpactNormal * 300.0f, FColor::Blue);
		this->SetActorLocation(backwardHitResult.GetComponent()->GetComponentLocation() + backwardHitResult.ImpactNormal * 130.0f + backwardHitResult.GetComponent()->GetUpVector() * 20.0f);
		CenterRotation = backwardHitResult.GetComponent()->GetComponentRotation() + FRotator(0, 0, 0);
		this->SetActorRotation(CenterRotation);
	}
}

void ACStructure_Ceiling::CheckLeft_Wall()
{
	FHitResult leftHitResult;
	FVector leftStartLocation = this->GetActorLocation();
	FVector leftEndLocation = this->GetActorLocation() + this->GetActorRightVector() * -200.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> leftObjectTypeQuery;
	leftObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel4));
	leftObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel5));

	TArray<AActor*> leftBoxActorsToIgnore;
	leftBoxActorsToIgnore.Add(this);

	bLeft_WallHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		leftStartLocation,
		leftEndLocation,
		leftObjectTypeQuery,
		false,
		leftBoxActorsToIgnore,
		EDrawDebugTrace::None,
		leftHitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red
	);

	if (bLeft_WallHit)
	{
		//DrawDebugLine(GetWorld(), leftHitResult.GetComponent()->GetComponentLocation(), leftHitResult.GetComponent()->GetComponentLocation() + leftHitResult.ImpactNormal * 300.0f, FColor::Blue);
		this->SetActorLocation(leftHitResult.GetComponent()->GetComponentLocation() + leftHitResult.ImpactNormal * 130.0f + leftHitResult.GetComponent()->GetUpVector() * 20.0f);
		CenterRotation = leftHitResult.GetComponent()->GetComponentRotation() + FRotator(0, 90, 0);
		this->SetActorRotation(CenterRotation);
	}
}

void ACStructure_Ceiling::CheckRight_Wall()
{
	FHitResult rightHitResult;
	FVector rightStartLocation = this->GetActorLocation();
	FVector rightEndLocation = this->GetActorLocation() + this->GetActorRightVector() * 200.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> rightObjectTypeQuery;
	rightObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel4));
	rightObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel5));

	TArray<AActor*> rightBoxActorsToIgnore;
	rightBoxActorsToIgnore.Add(this);

	bRight_WallHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		rightStartLocation,
		rightEndLocation,
		rightObjectTypeQuery,
		false,
		rightBoxActorsToIgnore,
		EDrawDebugTrace::None,
		rightHitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red
	);

	if (bRight_WallHit)
	{
		//DrawDebugLine(GetWorld(), rightHitResult.GetComponent()->GetComponentLocation(), rightHitResult.GetComponent()->GetComponentLocation() + rightHitResult.ImpactNormal * 300.0f, FColor::Blue);
		this->SetActorLocation(rightHitResult.GetComponent()->GetComponentLocation() + rightHitResult.ImpactNormal * 130.0f + rightHitResult.GetComponent()->GetUpVector() * 20.0f);
		CenterRotation = rightHitResult.GetComponent()->GetComponentRotation() + FRotator(0, -90, 0);
		this->SetActorRotation(CenterRotation);
	}
}

void ACStructure_Ceiling::CheckForward_Ceiling()
{
	FHitResult forwardHitResult;
	FVector forwardStartLocation = this->GetActorLocation();
	FVector forwardEndLocation = this->GetActorLocation() + this->GetActorForwardVector() * -200.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> forwardObjectTypeQuery;
	forwardObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel6));

	TArray<AActor*> forwardBoxActorsToIgnore;
	forwardBoxActorsToIgnore.Add(this);

	bForward_CeilingHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		forwardStartLocation,
		forwardEndLocation,
		forwardObjectTypeQuery,
		false,
		forwardBoxActorsToIgnore,
		EDrawDebugTrace::None,
		forwardHitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red
	);

	if (bForward_CeilingHit)
	{
		this->SetActorLocation(forwardHitResult.GetComponent()->GetComponentLocation() + forwardHitResult.ImpactNormal * 170.0f + forwardHitResult.GetComponent()->GetUpVector() * 20.0f);
		CenterRotation = forwardHitResult.GetComponent()->GetComponentRotation() + FRotator(0, 180, 0);
		this->SetActorRotation(CenterRotation);
	}
}

void ACStructure_Ceiling::CheckBackward_Ceiling()
{
	FHitResult backwardHitResult;
	FVector backwardStartLocation = this->GetActorLocation();
	FVector backwardEndLocation = this->GetActorLocation() + this->GetActorForwardVector() * 200.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> backwardObjectTypeQuery;
	backwardObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel6));

	TArray<AActor*> backwardBoxActorsToIgnore;
	backwardBoxActorsToIgnore.Add(this);

	bBackward_CeilingHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		backwardStartLocation,
		backwardEndLocation,
		backwardObjectTypeQuery,
		false,
		backwardBoxActorsToIgnore,
		EDrawDebugTrace::None,
		backwardHitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red
	);

	if (bBackward_CeilingHit)
	{
		this->SetActorLocation(backwardHitResult.GetComponent()->GetComponentLocation() + backwardHitResult.ImpactNormal * 170.0f + backwardHitResult.GetComponent()->GetUpVector() * 20.0f);
		CenterRotation = backwardHitResult.GetComponent()->GetComponentRotation() + FRotator(0, 0, 0);
		this->SetActorRotation(CenterRotation);
	}
}

void ACStructure_Ceiling::CheckLeft_Ceiling()
{
	FHitResult leftHitResult;
	FVector leftStartLocation = this->GetActorLocation();
	FVector leftEndLocation = this->GetActorLocation() + this->GetActorRightVector() * -200.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> leftObjectTypeQuery;
	leftObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel6));

	TArray<AActor*> leftBoxActorsToIgnore;
	leftBoxActorsToIgnore.Add(this);

	bLeft_CeilingHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		leftStartLocation,
		leftEndLocation,
		leftObjectTypeQuery,
		false,
		leftBoxActorsToIgnore,
		EDrawDebugTrace::None,
		leftHitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red
	);

	if (bLeft_CeilingHit)
	{
		this->SetActorLocation(leftHitResult.GetComponent()->GetComponentLocation() + leftHitResult.ImpactNormal * 170.0f + leftHitResult.GetComponent()->GetUpVector() * 20.0f);
		CenterRotation = leftHitResult.GetComponent()->GetComponentRotation() + FRotator(0, 90, 0);
		this->SetActorRotation(CenterRotation);
	}
}

void ACStructure_Ceiling::CheckRight_Ceiling()
{
	FHitResult rightHitResult;
	FVector rightStartLocation = this->GetActorLocation();
	FVector rightEndLocation = this->GetActorLocation() + this->GetActorRightVector() * 200.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> rightObjectTypeQuery;
	rightObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel6));

	TArray<AActor*> rightBoxActorsToIgnore;
	rightBoxActorsToIgnore.Add(this);

	bRight_CeilingHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		rightStartLocation,
		rightEndLocation,
		rightObjectTypeQuery,
		false,
		rightBoxActorsToIgnore,
		EDrawDebugTrace::None,
		rightHitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red
	);

	if (bRight_CeilingHit)
	{
		this->SetActorLocation(rightHitResult.GetComponent()->GetComponentLocation() + rightHitResult.ImpactNormal * 170.0f + rightHitResult.GetComponent()->GetUpVector() * 20.0f);
		CenterRotation = rightHitResult.GetComponent()->GetComponentRotation() + FRotator(0, -90, 0);
		this->SetActorRotation(CenterRotation);
	}
}
