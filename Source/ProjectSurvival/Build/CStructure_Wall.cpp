#include "Build/CStructure_Wall.h"
#include "Character/CSurvivor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

ACStructure_Wall::ACStructure_Wall()
{
	PreviewBox = CreateDefaultSubobject<UBoxComponent>("PreviewBox");

	PreviewBox->SetupAttachment(StaticMesh);
}

void ACStructure_Wall::CheckCenter()
{
	FHitResult centerBoxHitResult;
	FVector centerBoxLocation = this->GetActorLocation();
	FVector centerBoxSize = FVector(135, 10, 135);
	FRotator centerBoxOrientation;
	if (!bDownHit)
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

void ACStructure_Wall::CheckDown()
{
	FHitResult downHitResult;
	FVector downStartLocation = this->GetActorLocation();
	FVector downEndLocation = this->GetActorLocation() + this->GetActorUpVector() * -250.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> downObjectTypeQuery;
	downObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3));
	TArray<AActor*> downBoxActorsToIgnore;
	downBoxActorsToIgnore.Add(this);

	bDownHit = UKismetSystemLibrary::LineTraceSingleForObjects(
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

	if (bDownHit)
	{
		DrawDebugLine(GetWorld(), downHitResult.GetComponent()->GetComponentLocation(), downHitResult.GetComponent()->GetComponentLocation() + downHitResult.ImpactNormal * 300.0f, FColor::Blue);
		this->SetActorLocation(downHitResult.GetComponent()->GetComponentLocation() + downHitResult.GetComponent()->GetForwardVector() * 15.0f + downHitResult.ImpactNormal * 225.0f);
		CenterRotation = downHitResult.GetComponent()->GetComponentRotation() + FRotator(0, -90, 0);
		this->SetActorRotation(CenterRotation);
	}
}
