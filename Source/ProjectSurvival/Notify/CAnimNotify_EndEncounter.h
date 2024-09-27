// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_EndEncounter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCAnimNotify_EndEncounter : public UAnimNotify
{
	GENERATED_BODY()
public:
	FString GetNotifyName_Implementation() const override;
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
