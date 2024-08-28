#pragma once

#include "CoreMinimal.h"
#include "Build/CStructure.h"
#include "CStructure_Wall.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACStructure_Wall : public ACStructure
{
	GENERATED_BODY()
	
public:
	ACStructure_Wall();

public:
	void CheckCenter();
	bool GetWallCenterHit() { return bCenterHit; }

	void CheckDown_Foundation();
	bool GetWallDown_FoundationHit() { return bDown_FoundationHit; }

	void CheckDown_Wall();
	bool GetWallDown_WallHit() { return bDown_WallHit; }

	void CheckDown_DoorFrame();
	bool GetWallDown_DoorFrameHit() { return bDown_DoorFrameHit; }

	void CheckDown_Ceiling();
	bool GetWallDown_CeilingHit() { return bDown_CeilingHit; }

protected:
	UPROPERTY(EditAnywhere)
		class UBoxComponent* UpBox;

private:
	bool bCenterHit;
	bool bDown_FoundationHit;
	bool bDown_WallHit;
	bool bDown_DoorFrameHit;
	bool bDown_CeilingHit;

	FRotator CenterRotation;
};
