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

protected:
	UPROPERTY(EditAnywhere)
		class UBoxComponent* PreviewBox;
	UPROPERTY(EditAnywhere)
		class UBoxComponent* UpBox;

private:
	bool bCenterHit;
	bool bDown_FoundationHit;
	bool bDown_WallHit;

	FRotator CenterRotation;
};
