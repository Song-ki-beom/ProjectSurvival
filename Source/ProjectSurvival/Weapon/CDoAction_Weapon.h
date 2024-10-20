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
	void SetDoActionDamage(float InStoneHarvestDamage, float InWoodHarvestDamage)
	{
		StoneHarvestDamage = InStoneHarvestDamage;
		WoodHarvestDamage = InWoodHarvestDamage;
	}

private:
	float HarvestDamage = 50.0f;

	float StoneHarvestDamage = 0.0f;
	float WoodHarvestDamage = 0.0f;

	UPROPERTY()
		class UBoxComponent* HitBox;
};
