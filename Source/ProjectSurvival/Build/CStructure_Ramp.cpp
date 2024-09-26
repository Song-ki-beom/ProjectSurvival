#include "Build/CStructure_Ramp.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LandScape.h"
#include "Utility/CDebug.h"

ACStructure_Ramp::ACStructure_Ramp()
{
	ForwardBox = CreateDefaultSubobject<UBoxComponent>("ForwardBox");

	ForwardBox->SetupAttachment(PickupMesh);
}

void ACStructure_Ramp::CheckHeight()
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
			RampHeight = floorHitResult.ImpactPoint.Z + 100.0f;
			bHeightHit = true;
		}
		else
		{
			RampHeight = GetOwner()->GetActorLocation().Z;
			bHeightHit = false;
		}
	}
	else
	{
		RampHeight = GetOwner()->GetActorLocation().Z;
		bHeightHit = false;
	}
}

void ACStructure_Ramp::CheckCenter()
{
	FHitResult centerBoxHitResult;
	FVector centerBoxLocation = this->GetActorLocation() + FVector(0, 0, 50);
	FVector centerBoxSize = FVector(145, 145, 50);
	FRotator centerBoxOrientation;
	if (!bForwardHit)
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

void ACStructure_Ramp::CheckForward()
{
	FHitResult forwardHitResult;
	FVector forwardStartLocation = ForwardBox->GetComponentLocation();
	FVector forwardEndLocation = ForwardBox->GetComponentLocation() + ForwardBox->GetForwardVector() * 250.0f;
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
		//DrawDebugLine(GetWorld(), forwardHitResult.GetComponent()->GetComponentLocation(), forwardHitResult.GetComponent()->GetComponentLocation() + forwardHitResult.ImpactNormal * 300.0f, FColor::Blue);
		this->SetActorLocation(forwardHitResult.GetComponent()->GetComponentLocation() + forwardHitResult.ImpactNormal * 171.0f);
		CenterRotation = forwardHitResult.ImpactNormal.GetSafeNormal().Rotation() + FRotator(0, -90, 0);
		this->SetActorRotation(CenterRotation);
	}
}