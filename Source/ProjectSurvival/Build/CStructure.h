// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CStructure.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACStructure : public AActor
{
	GENERATED_BODY()
	
public:	
	ACStructure();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
