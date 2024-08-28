#pragma once

#include "CoreMinimal.h"
#include "Build/CStructure.h"
#include "CStructure_Stair.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACStructure_Stair : public ACStructure
{
	GENERATED_BODY()
	
public:
	ACStructure_Stair();

public:
	void CheckCenter();
	bool GetStairCenterHit() { return bCenterHit; }

	void CheckDown_Foundation();
	bool GetStairDown_FoundationHit() { return bDown_FoundationHit; }

protected:
	UPROPERTY(EditAnywhere)
		class UBoxComponent* UpBox;

private:
	bool bCenterHit;
	bool bDown_FoundationHit;

	FRotator CenterRotation;
};
