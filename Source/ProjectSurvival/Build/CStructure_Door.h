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

protected:
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds);

public:
	void CheckCenter();
	bool GetDoorCenterHit() { return bCenterHit; }

	void CheckUp_DoorFrame();
	bool GetDoorUp_DoorFrameHit() { return bUp_DoorFrameHit; }

	virtual void DoBuildTypeInteract() override;

	UFUNCTION(NetMulticast, Reliable)
		void BroadcastOpenDoor();

	UFUNCTION(NetMulticast, Reliable)
		void BroadcastCloseDoor();

private:
	bool bCenterHit;
	bool bUp_DoorFrameHit;

	FRotator CenterRotation;

	UPROPERTY()
		class ACSurvivorController* PersonalSurvivorController;

	UPROPERTY(VisibleAnywhere)
		class USceneComponent* TempRoot;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* DoorMesh;

	bool bIsOpened = false;

	void PerformOpenDoor();
	void PerformCloseDoor();

	FTimerHandle OpenTimerHandle;
	FTimerHandle CloseTimerHandle;

	FTimerHandle WaitTimerHandle;
	float Angle;

	UPROPERTY(EditAnywhere)
		class USoundWave* OpenSound;
	UPROPERTY(EditAnywhere)
		class USoundWave* CloseSound;
	UPROPERTY(EditAnywhere)
		class USoundAttenuation* DoorSoundAttenuation;

};
