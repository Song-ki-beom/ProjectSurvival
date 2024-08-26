#pragma once

#include "CoreMinimal.h"
#include "Build/CStructure.h"
#include "CStructure_Door.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACStructure_Door : public ACStructure
{
	GENERATED_BODY()
	
public:
	ACStructure_Door();

public:
	void CheckCenter();
	bool GetDoorCenterHit() { return bCenterHit; }

	void CheckUp_DoorFrame();
	bool GetDoorUp_DoorFrameHit() { return bUp_DoorFrameHit; }

	void DestroyPreviewBox();

	void OpenDoor();
	void CloseDoor();

private:
	bool bCenterHit;
	bool bUp_DoorFrameHit;

	FRotator CenterRotation;
};
