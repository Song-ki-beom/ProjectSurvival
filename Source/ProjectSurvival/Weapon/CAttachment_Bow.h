// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/CAttachment.h"
#include "CAttachment_Bow.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API ACAttachment_Bow : public ACAttachment
{
	GENERATED_BODY()
public:
	ACAttachment_Bow();
protected:
	void BeginPlay() override;
public:
	void Tick(float DeltaTime) override;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:  //Component
	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere)
		class UPoseableMeshComponent* PoseableMesh;

private:  // View MinMax
	UPROPERTY(EditDefaultsOnly, Category = "View")
		FVector2D ViewPitchRange = FVector2D(-40.0f, 30.0f);

public: 
	void OnBeginEquip_Implementation() override;
	void OnUnEquip_Implementation()   override;

public:
	float* GetBend();
private:
	FVector2D OriginViewPitchRange;
};
