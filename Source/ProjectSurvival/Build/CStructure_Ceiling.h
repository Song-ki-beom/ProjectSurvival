#pragma once

#include "CoreMinimal.h"
#include "Build/CStructure.h"
#include "CStructure_Ceiling.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACStructure_Ceiling : public ACStructure
{
	GENERATED_BODY()
	
public:
	ACStructure_Ceiling();

public:
	void CheckCenter();
	bool GetCeilingCenterHit() { return bCenterHit; }

	void CheckForward();
	bool GetCeilingForwardHit() { return bForwardHit; }
	
	void CheckBackward();
	bool GetCeilingBackwardHit() { return bBackwardHit; }

	void CheckLeft();
	bool GetCeilingLeftHit() { return bLeftHit; }

	void CheckRight();
	bool GetCeilingRightHit() { return bRightHit; }

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

private:
	bool bCenterHit;
	bool bForwardHit;
	bool bBackwardHit;
	bool bLeftHit;
	bool bRightHit;

	FRotator CenterRotation;
};
