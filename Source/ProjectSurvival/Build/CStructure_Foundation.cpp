#include "Build/CStructure_Foundation.h"
#include "Character/CSurvivor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LandScape.h"
#include "DrawDebugHelpers.h"
#include "Utility/CDebug.h"

ACStructure_Foundation::ACStructure_Foundation()
{
	//PrimaryActorTick.bCanEverTick = true;
	ForwardBox = CreateDefaultSubobject<UBoxComponent>("ForwardBox");
	BackwardBox = CreateDefaultSubobject<UBoxComponent>("BackwardBox");
	LeftBox = CreateDefaultSubobject<UBoxComponent>("LeftBox");
	RightBox = CreateDefaultSubobject<UBoxComponent>("RightBox");
	
	ForwardBox->SetupAttachment(StaticMesh);
	BackwardBox->SetupAttachment(StaticMesh);
	LeftBox->SetupAttachment(StaticMesh);
	RightBox->SetupAttachment(StaticMesh);
}

//void ACStructure_Foundation::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//
//}

void ACStructure_Foundation::CheckHeight()
{
	// 높이 체크
	FHitResult floorHitResult;
	float startLocationX = this->GetActorLocation().X;
	float startLocationY = this->GetActorLocation().Y;
	float startLocationZ = GetOwner()->GetActorLocation().Z;
	FVector startLocation = FVector(startLocationX, startLocationY, startLocationZ);
	FVector endLocation = startLocation - FVector(0, 0, 300);
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypeQuery;
	objectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));
	TArray<AActor*> ignores;
	FCollisionObjectQueryParams objectQueryParams;
	objectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1); // 에디터 상에서 LandSacpe > Custom > ObjectType Landscape로 변경할것
	FCollisionQueryParams collisionParams;
	bHeightHit = UKismetSystemLibrary::LineTraceSingleForObjects
	(
		GetWorld(),
		startLocation,
		endLocation,
		objectTypeQuery,
		false,
		ignores,
		EDrawDebugTrace::ForDuration,
		floorHitResult,
		true,
		FLinearColor::Yellow,
		FLinearColor::Red
	);

	AActor* heightHitActor = floorHitResult.GetActor();
	if (IsValid(heightHitActor))
	{
		if (heightHitActor->IsA(ALandscape::StaticClass()))
		{
			FoundationHeight = floorHitResult.ImpactPoint.Z + 100.0f;
			bHeightHit = true;
		}
		else
		{
			FoundationHeight = GetOwner()->GetActorLocation().Z;
			bHeightHit = false;
		}
	}
	else
	{
		FoundationHeight = GetOwner()->GetActorLocation().Z;
		bHeightHit = false;
	}

	//CDebug::Print("bHeightHit : ", bHeightHit);
}

void ACStructure_Foundation::CheckCenter()
{
	FHitResult centerBoxHitResult;
	FVector centerBoxLocation = this->GetActorLocation();
	FVector centerBoxSize = FVector(145, 145, 75);
	FRotator centerBoxOrientation;
	if (!bRightHit && !bLeftHit && !bBackwardHit && !bForwardHit)
		centerBoxOrientation = GetOwner()->GetActorRotation();
	else
		centerBoxOrientation = CenterRotation;
	ETraceTypeQuery centerBoxTraceTypeQuery = ETraceTypeQuery::TraceTypeQuery2; // 구체적으로 뭔지? TraceTypeQuery1하면 캐릭터를 감지안함
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

void ACStructure_Foundation::CheckRight()
{
	FHitResult rightHitResult;
	FVector rightStartLocation = this->GetActorLocation();
	FVector rightEndLocation = this->GetActorLocation() + this->GetActorRightVector() * 250.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> rightObjectTypeQuery;
	rightObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3));
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
		DrawDebugLine(GetWorld(), rightHitResult.GetComponent()->GetComponentLocation(), rightHitResult.GetComponent()->GetComponentLocation() + rightHitResult.ImpactNormal * 300.0f, FColor::Blue);
		this->SetActorLocation(rightHitResult.GetComponent()->GetComponentLocation() + rightHitResult.ImpactNormal * 171.0f);
		CenterRotation = rightHitResult.ImpactNormal.GetSafeNormal().Rotation() + FRotator(0,90,0);
		this->SetActorRotation(CenterRotation);
	}
}

void ACStructure_Foundation::CheckLeft()
{
	FHitResult leftHitResult;
	FVector leftStartLocation = this->GetActorLocation();
	FVector leftEndLocation = this->GetActorLocation() + this->GetActorRightVector() * -250.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> leftObjectTypeQuery;
	leftObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3));
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
		DrawDebugLine(GetWorld(), leftHitResult.GetComponent()->GetComponentLocation(), leftHitResult.GetComponent()->GetComponentLocation() + leftHitResult.ImpactNormal * 300.0f, FColor::Blue);
		this->SetActorLocation(leftHitResult.GetComponent()->GetComponentLocation() + leftHitResult.ImpactNormal * 171.0f);
		CenterRotation = leftHitResult.ImpactNormal.GetSafeNormal().Rotation() + FRotator(0, -90, 0);
		this->SetActorRotation(CenterRotation);
	}
}

void ACStructure_Foundation::CheckBackward()
{
	FHitResult backwardHitResult;
	FVector backwardStartLocation = this->GetActorLocation();
	FVector backwardEndLocation = this->GetActorLocation() + this->GetActorForwardVector() * -250.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> backwardObjectTypeQuery;
	backwardObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3));
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
		DrawDebugLine(GetWorld(), backwardHitResult.GetComponent()->GetComponentLocation(), backwardHitResult.GetComponent()->GetComponentLocation() + backwardHitResult.ImpactNormal * 300.0f, FColor::Blue);
		this->SetActorLocation(backwardHitResult.GetComponent()->GetComponentLocation() + backwardHitResult.ImpactNormal * 171.0f);
		CenterRotation = backwardHitResult.ImpactNormal.GetSafeNormal().Rotation();// +FRotator(0, 0, 0);
		this->SetActorRotation(CenterRotation);
	}
}

void ACStructure_Foundation::CheckForward()
{
	FHitResult forwardHitResult;
	FVector forwardStartLocation = this->GetActorLocation();
	FVector forwardEndLocation = this->GetActorLocation() + this->GetActorForwardVector() * 250.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> forwardObjectTypeQuery;
	forwardObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3));
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
		DrawDebugLine(GetWorld(), forwardHitResult.GetComponent()->GetComponentLocation(), forwardHitResult.GetComponent()->GetComponentLocation() + forwardHitResult.ImpactNormal * 300.0f, FColor::Blue);
		this->SetActorLocation(forwardHitResult.GetComponent()->GetComponentLocation() + forwardHitResult.ImpactNormal * 171.0f);
		CenterRotation = forwardHitResult.ImpactNormal.GetSafeNormal().Rotation() + FRotator(0, 180, 0);
		this->SetActorRotation(CenterRotation);
	}
}
