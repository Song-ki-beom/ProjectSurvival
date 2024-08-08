// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CBuildStructure.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACBuildStructure : public AActor
{
	GENERATED_BODY()
	
public:	
	ACBuildStructure();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
