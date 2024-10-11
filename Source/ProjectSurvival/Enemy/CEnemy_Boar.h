// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/CEnemy.h"
#include "CEnemy_Boar.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API ACEnemy_Boar : public ACEnemy
{
	GENERATED_BODY()
	
public:
	ACEnemy_Boar();
	virtual float DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

protected:

protected:
	virtual void BeginPlay() override;

};
