#pragma once

#include "CoreMinimal.h"
#include "Build/CStructure.h"
#include "CStructure_Placeable.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACStructure_Placeable : public ACStructure
{
	GENERATED_BODY()
	
public:
	void CheckCenter();
	bool GetWallCenterHit() { return bCenterHit; }

	void CheckDown_Foundation();
	bool GetWorkingBenchDown_FoundationHit() { return bDown_FoundationHit; }

	void CheckDown_Ceiling();
	bool GetWorkingBenchDown_CeilingHit() { return bDown_CeilingHit; }

private:
	bool bCenterHit;
	bool bDown_FoundationHit;
	bool bDown_CeilingHit;

	FRotator CenterRotation;
};
