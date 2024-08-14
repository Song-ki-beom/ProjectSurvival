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

void ACStructure_Foundation::DoTraceFoundation(float& OutHeightValue, bool& InIsBuildable)
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
			OutHeightValue = floorHitResult.ImpactPoint.Z + 50.0f;
			bHeightHit = true;
		}
		else
		{
			OutHeightValue = GetOwner()->GetActorLocation().Z;
			bHeightHit = false;
		}
	}
	else
	{
		OutHeightValue = GetOwner()->GetActorLocation().Z;
		bHeightHit = false;
	}

	FHitResult boxHitResult;
	FVector boxLocation = this->GetActorLocation() + FVector(0, 0, 50);
	FVector boxSize = FVector(150, 150, 10);
	FRotator orientation = GetOwner()->GetActorRotation();
	ETraceTypeQuery traceTypeQuery = ETraceTypeQuery::TraceTypeQuery2; // 구체적으로 뭔지? TraceTypeQuery1하면 캐릭터를 감지안함
	bool bTraceComplex = false;
	TArray<AActor*> actorsToIgnore;
	bool bShowDebug = true;
	TArray<FHitResult> centerHitResults;
	bool bCenterHit = UKismetSystemLibrary::BoxTraceSingle(
		GetWorld(),
		boxLocation,
		boxLocation,
		boxSize,
		orientation,
		traceTypeQuery,
		bTraceComplex,
		actorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		boxHitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red
	);
		
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
