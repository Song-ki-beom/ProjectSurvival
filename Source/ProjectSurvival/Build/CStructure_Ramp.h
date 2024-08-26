#pragma once

#include "CoreMinimal.h"
#include "Build/CStructure.h"
#include "CStructure_Ramp.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACStructure_Ramp : public ACStructure
{
	GENERATED_BODY()
	
public:
	ACStructure_Ramp();

public:
	void CheckHeight();
	bool GetRampHeightHit() { return bHeightHit; }
	float GetRampHeight() { return RampHeight; }

	void CheckCenter();
	bool GetRampCenterHit() { return bCenterHit; }
	 
	void CheckForward();
	bool GetRampForwardHit() { return bForwardHit; }
		 
	void DestroyPreviewBox();

protected:
	UPROPERTY(EditAnywhere)
		class UBoxComponent* PreviewBox;
	UPROPERTY(EditAnywhere)
		class UBoxComponent* ForwardBox;

	float RampHeight;
	bool bHeightHit;
	bool bCenterHit;
	bool bForwardHit;

	FRotator CenterRotation;
};
