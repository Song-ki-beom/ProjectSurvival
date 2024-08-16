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
	void CheckHeight();
	bool GetFoundationHeightHit() { return bHeightHit; }
	float GetFoundationHeight() { return FoundationHeight; }

	void CheckCenter();
	bool GetFoundationCenterHit() { return bCenterHit; }

	void CheckRight();
	bool GetFoundationRightHit() { return bRightHit; }

protected:
//	UFUNCTION()
//		virtual void FloorCheckBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
//	UFUNCTION()
//		virtual void FloorCheckBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:
	UPROPERTY(EditAnywhere)
		class UBoxComponent* TopBox;
	UPROPERTY(EditAnywhere)
		class UBoxComponent* BottomBox;
	UPROPERTY(EditAnywhere)
		class UBoxComponent* LeftBox;
	UPROPERTY(EditAnywhere)
		class UBoxComponent* RightBox;
	UPROPERTY()
		UPrimitiveComponent* RightComponent;

	TArray<AActor*> FloorCheckBoxOverlapActors;

	float FoundationHeight;
	bool bHeightHit;
	bool bCenterHit;
	bool bRightHit;

	FRotator CenterRotation;
};
