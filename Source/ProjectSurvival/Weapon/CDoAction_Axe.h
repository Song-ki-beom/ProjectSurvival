// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/CDoAction.h"
#include "CDoAction_Axe.generated.h"


UCLASS()
class PROJECTSURVIVAL_API UCDoAction_Axe : public UCDoAction
{
	GENERATED_BODY()
	

public:
	void DoAction()         override;
	void Begin_DoAction()   override;

	//void End_DoAction()     override;
	//void OnUnEquip()		override;
	void Slash();
	


private:
	float HarvestDamage = 50.0f;

};
