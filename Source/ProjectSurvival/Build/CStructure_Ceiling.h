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

	void CheckDown_Forward();
	bool GetCeilingDown_ForwardHit() { return bDown_ForwardHit; }
	
	void CheckDown_Backward();
	bool GetCeilingDown_BackwardHit() { return bDown_BackwardHit; }

	void CheckDown_Left();
	bool GetCeilingDown_LeftHit() { return bDown_LeftHit; }

	void CheckDown_Right();
	bool GetCeilingDown_RightHit() { return bDown_RightHit; }

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
	bool bDown_ForwardHit;
	bool bDown_BackwardHit;
	bool bDown_LeftHit;
	bool bDown_RightHit;

	FRotator CenterRotation;
};
