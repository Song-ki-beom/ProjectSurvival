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



//protected:
//	virtual void Tick(float DeltaTime) override;

public:
	void CheckHeight();
	bool GetFoundationHeightHit() { return bHeightHit; }
	float GetFoundationHeight() { return FoundationHeight; }

	void CheckCenter();
	bool GetFoundationCenterHit() { return bCenterHit; }

	void CheckRight();
	bool GetFoundationRightHit() { return bRightHit; }

	void CheckLeft();
	bool GetFoundationLeftHit() { return bLeftHit; }

	void CheckBackward();
	bool GetFoundationBackwardHit() { return bBackwardHit; }

	void CheckForward();
	bool GetFoundationForwardHit() { return bForwardHit; }

	void DestroyPreviewBox();

//protected:
//	UFUNCTION()
//		virtual void OnPreviewBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
//	UFUNCTION()
//		virtual void OnPreviewBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:
	UPROPERTY(EditAnywhere)
		class UBoxComponent* PreviewBox;
	UPROPERTY(EditAnywhere)
		class UBoxComponent* ForwardBox;
	UPROPERTY(EditAnywhere)
		class UBoxComponent* BackwardBox;
	UPROPERTY(EditAnywhere)
		class UBoxComponent* LeftBox;
	UPROPERTY(EditAnywhere)
		class UBoxComponent* RightBox;
	//UPROPERTY()
	//	UPrimitiveComponent* RightComponent;

	float FoundationHeight;
	bool bHeightHit;
	bool bCenterHit;
	bool bRightHit;
	bool bLeftHit;
	bool bBackwardHit;
	bool bForwardHit;

	FRotator CenterRotation;
};
