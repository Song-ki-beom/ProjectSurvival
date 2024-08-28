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

	void CheckForward_Wall();
	bool GetCeilingForward_WallHit() { return bForward_WallHit; }

	void CheckBackward_Wall();
	bool GetCeilingBackward_WallHit() { return bBackward_WallHit; }

	void CheckLeft_Wall();
	bool GetCeilingLeft_WallHit() { return bLeft_WallHit; }

	void CheckRight_Wall();
	bool GetCeilingRight_WallHit() { return bRight_WallHit; }

	void CheckForward_Ceiling();
	bool GetCeilingForward_CeilingHit() { return bForward_CeilingHit; }

	void CheckBackward_Ceiling();
	bool GetCeilingBackward_CeilingHit() { return bBackward_CeilingHit; }

	void CheckLeft_Ceiling();
	bool GetCeilingLeft_CeilingHit() { return bLeft_CeilingHit; }

	void CheckRight_Ceiling();
	bool GetCeilingRight_CeilingHit() { return bRight_CeilingHit; }

protected:
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
	bool bForward_WallHit;
	bool bBackward_WallHit;
	bool bLeft_WallHit;
	bool bRight_WallHit;
	bool bForward_CeilingHit;
	bool bBackward_CeilingHit;
	bool bLeft_CeilingHit;
	bool bRight_CeilingHit;

	FRotator CenterRotation;
};
