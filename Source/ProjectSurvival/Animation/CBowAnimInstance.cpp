// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/CBowAnimInstance.h"

void UCBowAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	bIsAiming = false;

}
void UCBowAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
		
}
