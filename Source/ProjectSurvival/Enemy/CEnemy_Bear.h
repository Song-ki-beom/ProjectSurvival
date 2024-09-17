// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/CEnemy.h"
#include "CEnemy_Bear.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API ACEnemy_Bear : public ACEnemy
{
	GENERATED_BODY()

public:
	ACEnemy_Bear();

protected:
	virtual void BeginPlay() override;

};
