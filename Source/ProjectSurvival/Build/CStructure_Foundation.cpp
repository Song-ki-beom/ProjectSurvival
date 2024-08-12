#include "Build/CStructure_Foundation.h"
#include "DrawDebugHelpers.h"
#include "Utility/CDebug.h"

ACStructure_Foundation::ACStructure_Foundation()
{
	PrimaryActorTick.bCanEverTick = true;
	FloorCheckBox = CreateDefaultSubobject<UBoxComponent>("FloorBox");
	FloorCheckBox->SetupAttachment(StaticMesh);
	FloorCheckBox->OnComponentBeginOverlap.AddDynamic(this, &ACStructure_Foundation::FloorCheckBoxBeginOverlap);
	FloorCheckBox->OnComponentEndOverlap.AddDynamic(this, &ACStructure_Foundation::FloorCheckBoxEndOverlap);
	FloorCheckBox->SetGenerateOverlapEvents(true);
}

void ACStructure_Foundation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACStructure_Foundation::GetFloorHeight(float& OutHeightValue, bool& OutHitResult)
{
	FHitResult hitResult;
	float startLocationX = FloorCheckBox->GetComponentLocation().X;
	float startLocationY = FloorCheckBox->GetComponentLocation().Y;
	float startLocationZ = GetOwner()->GetActorLocation().Z;
	FVector startLocation = FVector(startLocationX, startLocationY, startLocationZ);
	FVector endLocation = startLocation - FVector(0, 0, 200);
	DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Red, false, 10.0f, 0, 1.0f);
	GetWorld()->LineTraceSingleByChannel(hitResult, startLocation, endLocation, ECollisionChannel::ECC_Visibility);
	if (IsValid(hitResult.GetActor()))
	{
		//CDebug::Print("HitResult : ", hitResult.GetActor());
		OutHeightValue = hitResult.ImpactPoint.Z + 50.0f;
		if (FloorCheckBoxOverlapActors.Num() == 0)
		{
			OutHitResult = true;
		}
		else
		{
			OutHitResult = false;
		}
	}
	else
	{
		//CDebug::Print("HitResult : None");
		OutHeightValue = GetOwner()->GetActorLocation().Z;
		OutHitResult = false;
	}
}

void ACStructure_Foundation::FloorCheckBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		CDebug::Print("Overlapped : ", OtherActor);
		FloorCheckBoxOverlapActors.Add(OtherActor);
	}
	else
	{
		CDebug::Print("Not Valid");
	}
}

void ACStructure_Foundation::FloorCheckBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsValid(OtherActor))
	{
		CDebug::Print("UnOverlapped : ", OtherActor);
		FloorCheckBoxOverlapActors.Remove(OtherActor);
	}
	else
	{
		CDebug::Print("Not Valid");
	}
}
