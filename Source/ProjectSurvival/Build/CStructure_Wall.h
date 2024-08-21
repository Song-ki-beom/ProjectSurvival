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

	void CheckDown();
	bool GetWallDownHit() { return bDownHit; }

protected:
	UPROPERTY(EditAnywhere)
		class UBoxComponent* PreviewBox;

private:
	bool bCenterHit;
	bool bDownHit;

	FRotator CenterRotation;
};
