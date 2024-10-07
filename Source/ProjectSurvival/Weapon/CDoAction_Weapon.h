// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/CDoAction.h"
#include "CDoAction_Weapon.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCDoAction_Weapon : public UCDoAction
{
	GENERATED_BODY()
	
public:
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;
	void WeaponHitTrace();

private:
	float HarvestDamage = 50.0f;
	//float TraceDistance = 45.0f;
	//float TraceOffset = 100.0f;

	class UBoxComponent* HitBox;
};
