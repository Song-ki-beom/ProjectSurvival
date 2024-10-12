// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CBowAnimInstance.generated.h"

UCLASS()
class PROJECTSURVIVAL_API UCBowAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

protected: 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		float Bend = 0.0f;

public: 
	FORCEINLINE float* GetBend() { return &Bend; }
	FORCEINLINE void   SetBend(float InBend) { Bend = InBend; }
	


public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool bIsAiming;

	FORCEINLINE bool GetIsAiming() {
		return bIsAiming;
	}
	FORCEINLINE void SetAiming(bool bInIsAiming) 
	{bIsAiming = bInIsAiming;}
};
