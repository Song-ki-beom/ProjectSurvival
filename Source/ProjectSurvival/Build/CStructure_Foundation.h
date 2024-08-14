#pragma once

#include "CoreMinimal.h"
#include "Build/CStructure.h"
#include "CStructure_Foundation.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACStructure_Foundation : public ACStructure
{
	GENERATED_BODY()

public:
	ACStructure_Foundation();



protected:
	virtual void Tick(float DeltaTime) override;

public:
	//UBoxComponent* GetFloorBox() { return FloorCheckBox; }
	void DoTraceFoundation(FVector& InLocation, FRotator& InRotation, bool& InIsBuildable, bool& InIsSnapped);

protected:
//	UFUNCTION()
//		virtual void FloorCheckBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
//	UFUNCTION()
//		virtual void FloorCheckBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:
	UPROPERTY(EditAnywhere)
		class UBoxComponent* FloorCheckBox;

	TArray<AActor*> FloorCheckBoxOverlapActors;

	AActor* RightFoundation;

	
};
