// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_AttenuSoundPlay.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCAnimNotify_AttenuSoundPlay : public UAnimNotify
{
	GENERATED_BODY()
public:
	FString GetNotifyName_Implementation() const override;
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	UPROPERTY(EditAnywhere)
		class USoundAttenuation* SoundAttenuation;
	UPROPERTY(EditAnywhere)
		class USoundWave* Sound;
	UPROPERTY(EditAnywhere)
		float SoundMultiplier=1.0f;
};
