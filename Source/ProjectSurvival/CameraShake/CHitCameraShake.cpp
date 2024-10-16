// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraShake/CHitCameraShake.h"

UCHitCameraShake::UCHitCameraShake()
{
	OscillationDuration = 0.20f;
	OscillationBlendInTime = 0.05f;
	OscillationBlendOutTime = 0.05f;

	RotOscillation.Pitch.Amplitude = FMath::RandRange(2.5f, 5.0f);
	RotOscillation.Pitch.Frequency = FMath::RandRange(15.0f, 25.0f);

	RotOscillation.Yaw.Amplitude = FMath::RandRange(3.0f, 7.0f);
	RotOscillation.Yaw.Frequency = FMath::RandRange(15.0f, 25.0f);


}