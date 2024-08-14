#include "Build/CStructure_Foundation.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LandScape.h"
#include "DrawDebugHelpers.h"
#include "Utility/CDebug.h"

ACStructure_Foundation::ACStructure_Foundation()
{
	PrimaryActorTick.bCanEverTick = true;
	//FloorCheckBox = CreateDefaultSubobject<UBoxComponent>("FloorBox");
	//FloorCheckBox->SetupAttachment(StaticMesh);
	//FloorCheckBox->OnComponentBeginOverlap.AddDynamic(this, &ACStructure_Foundation::FloorCheckBoxBeginOverlap);
	//FloorCheckBox->OnComponentEndOverlap.AddDynamic(this, &ACStructure_Foundation::FloorCheckBoxEndOverlap);
	//FloorCheckBox->SetGenerateOverlapEvents(true);
}

void ACStructure_Foundation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ACStructure_Foundation::DoTraceFoundation(FVector& InLocation, FRotator& InRotation, bool& InIsBuildable, bool& InIsSnapped)
{
	// 높이 체크
	FHitResult floorHitResult;
	float startLocationX = this->GetActorLocation().X;
	float startLocationY = this->GetActorLocation().Y;
	float startLocationZ = GetOwner()->GetActorLocation().Z;
	FVector startLocation = FVector(startLocationX, startLocationY, startLocationZ);
	FVector endLocation = startLocation - FVector(0, 0, 200);
	DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Red, false, 10.0f, 0, 1.0f);
	FCollisionObjectQueryParams objectQueryParams;
	objectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1); // 에디터 상에서 LandSacpe > Custom > ObjectType Landscape로 변경할것
	FCollisionQueryParams collisionParams;
	bool bHeightHit = GetWorld()->LineTraceSingleByObjectType(floorHitResult, startLocation, endLocation, objectQueryParams);

	AActor* heightHitActor = floorHitResult.GetActor();
	if (IsValid(heightHitActor))
	{
		if (heightHitActor->IsA(ALandscape::StaticClass()))
		{
			InLocation.Z = floorHitResult.ImpactPoint.Z + 50.0f;
			bHeightHit = true;
		}
		else
		{
			InLocation.Z = GetOwner()->GetActorLocation().Z;
			bHeightHit = false;
		}
	}
	else
	{
		InLocation.Z = GetOwner()->GetActorLocation().Z;
		bHeightHit = false;
	}

	// 가운데 체크
	FHitResult centerBoxHitResult;
	FVector centerBoxLocation = this->GetActorLocation() + FVector(0, 0, 50);
	FVector centerBoxSize = FVector(150, 150, 50);
	FRotator centerBoxOrientation = GetOwner()->GetActorRotation();
	ETraceTypeQuery centerBoxTraceTypeQuery = ETraceTypeQuery::TraceTypeQuery2; // 구체적으로 뭔지? TraceTypeQuery1하면 캐릭터를 감지안함
	bool bCenterBoxTraceComplex = false;
	TArray<AActor*> centerBoxActorsToIgnore;
	TArray<FHitResult> centerBoxHitResults;
	bool bCenterHit = UKismetSystemLibrary::BoxTraceSingle(
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
		
	// 오른쪽 체크
	FHitResult rightBoxHitResult;
	FVector rightBoxOffset = FVector(0, 200, 0);
	FVector rightBoxLocation = centerBoxLocation + centerBoxOrientation.RotateVector(rightBoxOffset);
	FVector rightBoxSize = FVector(50, 50, 50);
	FRotator rightBoxOrientation = FRotator::ZeroRotator;
	if (IsValid(RightFoundation))
	{
		rightBoxOrientation = RightFoundation->GetActorRotation();
	}
	else
	{
		rightBoxOrientation = centerBoxOrientation;
	}
	TArray<TEnumAsByte<EObjectTypeQuery>> offsetBoxObjectTypes;
	offsetBoxObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));
	bool bOffsetBoxTraceComplex = false;
	TArray<AActor*> rightBoxActorsToIgnore;
	bool bRightHit = UKismetSystemLibrary::BoxTraceSingleForObjects(
		GetWorld(),
		rightBoxLocation,
		rightBoxLocation,
		rightBoxSize,
		rightBoxOrientation,
		offsetBoxObjectTypes,
		bOffsetBoxTraceComplex,
		rightBoxActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		rightBoxHitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red
	);

	if (bRightHit)
	{
		RightFoundation = rightBoxHitResult.Actor.Get();
		InIsSnapped = true;
		FVector actorLocation = rightBoxHitResult.Actor->GetActorLocation();
		InRotation = rightBoxHitResult.Actor->GetActorRotation();
		FVector offsetLocation = rightBoxHitResult.Actor->GetActorTransform().TransformPosition(FVector(-300,0,0));
		InLocation = offsetLocation;
	}
	else
	{
		RightFoundation = nullptr;
		InIsSnapped = false;
	}


	// 최종 결과
	InIsBuildable = (bHeightHit && !bCenterHit);
}

//void ACStructure_Foundation::FloorCheckBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (IsValid(OtherActor))
//	{
//		CDebug::Print("Overlapped : ", OtherActor);
//		FloorCheckBoxOverlapActors.Add(OtherActor);
//	}
//	else
//	{
//		CDebug::Print("Not Valid");
//	}
//}
//
//void ACStructure_Foundation::FloorCheckBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	if (IsValid(OtherActor))
//	{
//		CDebug::Print("UnOverlapped : ", OtherActor);
//		FloorCheckBoxOverlapActors.Remove(OtherActor);
//	}
//	else
//	{
//		CDebug::Print("Not Valid");
//	}
//}
