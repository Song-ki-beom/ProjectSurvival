#include "Build/CStructure_Wall.h"
#include "Character/CSurvivor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "Utility/CDebug.h"

ACStructure_Wall::ACStructure_Wall()
{
	UpBox = CreateDefaultSubobject<UBoxComponent>("UpBox");

	UpBox->SetupAttachment(PickupMesh);
}

void ACStructure_Wall::CheckCenter()
{
	FHitResult centerBoxHitResult;
	FVector centerBoxLocation = this->GetActorLocation();
	FVector centerBoxSize = FVector(135, 10, 135);
	FRotator centerBoxOrientation;
	if (!bDown_FoundationHit && !bDown_WallHit && !bDown_DoorFrameHit)
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

void ACStructure_Wall::CheckDown_Foundation()
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
		DrawDebugLine(GetWorld(), downHitResult.GetComponent()->GetComponentLocation(), downHitResult.GetComponent()->GetComponentLocation() + downHitResult.ImpactNormal * 300.0f, FColor::Blue);
		this->SetActorLocation(downHitResult.GetComponent()->GetComponentLocation() + downHitResult.GetComponent()->GetForwardVector() * 15.0f + downHitResult.ImpactNormal * 225.0f);
		CenterRotation = downHitResult.GetComponent()->GetComponentRotation() + FRotator(0, -90, 0);
		this->SetActorRotation(CenterRotation);
	}
}

void ACStructure_Wall::CheckDown_Wall()
{
	FHitResult downHitResult;
	FVector downStartLocation = this->GetActorLocation();
	FVector downEndLocation = this->GetActorLocation() + this->GetActorUpVector() * -250.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> downObjectTypeQuery;
	downObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel4));

	TArray<AActor*> downBoxActorsToIgnore;
	downBoxActorsToIgnore.Add(this);

	bDown_WallHit = UKismetSystemLibrary::LineTraceSingleForObjects(
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

	if (bDown_WallHit)
	{
		DrawDebugLine(GetWorld(), downHitResult.GetComponent()->GetComponentLocation(), downHitResult.GetComponent()->GetComponentLocation() + downHitResult.ImpactNormal * 300.0f, FColor::Blue);
		this->SetActorLocation(downHitResult.GetComponent()->GetComponentLocation() + downHitResult.GetComponent()->GetForwardVector() * 15.0f + downHitResult.ImpactNormal * 170.0f);
		CenterRotation = downHitResult.GetComponent()->GetComponentRotation() + FRotator(0, -90, 0);
		this->SetActorRotation(CenterRotation);
	}
}

void ACStructure_Wall::CheckDown_DoorFrame()
{
	FHitResult downHitResult;
	FVector downStartLocation = this->GetActorLocation();
	FVector downEndLocation = this->GetActorLocation() + this->GetActorUpVector() * -250.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> downObjectTypeQuery;
	downObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel5));

	TArray<AActor*> downBoxActorsToIgnore;
	downBoxActorsToIgnore.Add(this);

	bDown_DoorFrameHit = UKismetSystemLibrary::LineTraceSingleForObjects(
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

	if (bDown_DoorFrameHit)
	{
		DrawDebugLine(GetWorld(), downHitResult.GetComponent()->GetComponentLocation(), downHitResult.GetComponent()->GetComponentLocation() + downHitResult.ImpactNormal * 300.0f, FColor::Blue);
		this->SetActorLocation(downHitResult.GetComponent()->GetComponentLocation() + downHitResult.GetComponent()->GetForwardVector() * 15.0f + downHitResult.ImpactNormal * 170.0f);
		CenterRotation = downHitResult.GetComponent()->GetComponentRotation() + FRotator(0, -90, 0);
		this->SetActorRotation(CenterRotation);
	}
}