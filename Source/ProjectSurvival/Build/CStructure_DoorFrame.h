#pragma once

#include "CoreMinimal.h"
#include "Build/CStructure.h"
#include "CStructure_DoorFrame.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACStructure_DoorFrame : public ACStructure
{
	GENERATED_BODY()
	
public:
	ACStructure_DoorFrame();

public:
	void CheckCenter();
	bool GetDoorFrameCenterHit() { return bCenterHit; }

	void CheckDown_Foundation();
	bool GetDoorFrameDown_FoundationHit() { return bDown_FoundationHit; }

	void CheckDown_Ceiling();
	bool GetDoorFrameDown_CeilingHit() { return bDown_CeilingHit; }

	void DestroyPreviewBox();

protected:
	UPROPERTY(EditAnywhere)
		class UBoxComponent* UpBox;

private:
	bool bCenterHit;
	bool bDown_FoundationHit;
	bool bDown_CeilingHit;

	FRotator CenterRotation;
};
