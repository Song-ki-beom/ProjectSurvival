// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/CDoAction.h"
#include "Engine/DataTable.h"
#include "CDoAction_Axe.generated.h"
#define NO_INDEX -1


UCLASS()
class PROJECTSURVIVAL_API UCDoAction_Axe : public UCDoAction
{
	GENERATED_BODY()
	

public:
	void DoAction()         override;
	//void Begin_DoAction()   override;
	//void End_DoAction()     override;
	//void OnUnEquip()		override;
	void Slash();
	bool CheckIsFoliageInstance(const FHitResult& Hit);
	void SwitchFoligeToDestructible(FString* hitIndex);

private:
	float TraceDistance = 50.0f;
	UPROPERTY(EditAnywhere, Category = "Slash")
		UDataTable* DestructibleDataTable;
	FTransform SpawnTransform;
	int32 InstanceIndex = NO_INDEX;

};
