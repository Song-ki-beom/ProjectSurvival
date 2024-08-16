#include "Build/CStructure_Foundation.h"
#include "Character/CSurvivor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LandScape.h"
#include "DrawDebugHelpers.h"
#include "Utility/CDebug.h"

ACStructure_Foundation::ACStructure_Foundation()
{
	PrimaryActorTick.bCanEverTick = true;
	PreviewBox = CreateDefaultSubobject<UBoxComponent>("PreviewBox");
	TopBox = CreateDefaultSubobject<UBoxComponent>("TopBox");
	BottomBox = CreateDefaultSubobject<UBoxComponent>("BottomBox");
	LeftBox = CreateDefaultSubobject<UBoxComponent>("LeftBox");
	RightBox = CreateDefaultSubobject<UBoxComponent>("RightBox");
	
	PreviewBox->SetupAttachment(StaticMesh);
	TopBox->SetupAttachment(StaticMesh);
	BottomBox->SetupAttachment(StaticMesh);
	LeftBox->SetupAttachment(StaticMesh);
	RightBox->SetupAttachment(StaticMesh);
}

void ACStructure_Foundation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

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
			FoundationHeight = floorHitResult.ImpactPoint.Z + 50.0f;
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
	FVector centerBoxLocation = this->GetActorLocation() + FVector(0, 0, 50);
	FVector centerBoxSize = FVector(150, 150, 50);
	FRotator centerBoxOrientation;
	if (!bRightHit)
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
		//CDebug::Print("bRightHit : ", bRightHit);
		//CDebug::Print("HitComponent", rightHitResult.GetComponent());
	}
	else
	{
		//CDebug::Print("bRightHit : ", bRightHit);
	}
}

void ACStructure_Foundation::DestroyPreviewBox()
{
	PreviewBox->DestroyComponent();
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
