#include "Build/CStructure_Placeable.h"
#include "Build/CStructure_Foundation.h"
#include "Build/CStructure_Ceiling.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Utility/CDebug.h"

ACStructure_Placeable::ACStructure_Placeable()
{
	DownBox = CreateDefaultSubobject<UBoxComponent>("DownBox");
	DownBox->SetupAttachment(PickupMesh);
}
void ACStructure_Placeable::CheckCenter()
{
	FHitResult centerBoxHitResult;
	FVector centerBoxLocation = this->GetActorLocation();
	FVector centerBoxSize = FVector(145, 145, 75);
	FRotator centerBoxOrientation;
	if (!bDown_FoundationAndCeilingActorHit)
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

void ACStructure_Placeable::CheckDown_FoundationAndCeiling()
{
	// 액터 체크
	FHitResult foundationActorHitResult;
	FVector startLocation = DownBox->GetComponentLocation();
	FVector endLocation = startLocation - FVector(0, 0, 300);
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypeQuery;
	objectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECC_Visibility));
	TArray<AActor*> ignores;
	ignores.Add(this);
	FCollisionQueryParams collisionParams;
	bool bDownActorHit = UKismetSystemLibrary::LineTraceSingleForObjects
	(
		GetWorld(),
		startLocation,
		endLocation,
		objectTypeQuery,
		false,
		ignores,
		EDrawDebugTrace::ForDuration,
		foundationActorHitResult,
		true,
		FLinearColor::Yellow,
		FLinearColor::Red
	);

	if (bDownActorHit) // 라인트레이스가 어떤 액터에 히트했는지 확인
	{
		AActor* hitActor = foundationActorHitResult.GetActor();
		if (hitActor && (hitActor->IsA(ACStructure_Foundation::StaticClass()) || hitActor->IsA(ACStructure_Ceiling::StaticClass())))
		{
			CDebug::Print("hitActor: ", hitActor, FColor::Cyan);
			bDown_FoundationAndCeilingActorHit = true;
		}
		else
		{
			CDebug::Print("hitActor: ", hitActor, FColor::Cyan);
			bDown_FoundationAndCeilingActorHit = false;
		}
	}
}

//void ACStructure_Placeable::CheckDown_FoundationActor()
//{
//	
//}
//
//void ACStructure_Placeable::CheckDown_CeilingActor()
//{
//}
