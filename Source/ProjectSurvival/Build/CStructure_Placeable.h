#pragma once

#include "CoreMinimal.h"
#include "Build/CStructure.h"
#include "CStructure_Placeable.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACStructure_Placeable : public ACStructure
{
	GENERATED_BODY()
	
public:
	ACStructure_Placeable();

public:
	void CheckCenter();
	bool GetWallCenterHit() { return bCenterHit; }

	void CheckDown_FoundationActor();
	bool GetPlaceableDown_FoundationHit() { return bDown_FoundationActorHit; }

	void CheckDown_CeilingActor();
	bool GetPlaceableDown_CeilingHit() { return bDown_CeilingActorHit; }

protected:
	UPROPERTY(EditAnywhere)
		class UBoxComponent* DownBox;

private:
	bool bCenterHit;
	bool bDown_FoundationActorHit;
	bool bDown_CeilingActorHit;

	FRotator CenterRotation;
};
