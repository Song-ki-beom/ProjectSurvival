// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CLobbySurvivorState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API ACLobbySurvivorState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(ReplicatedUsing = OnRep_StateSurvivorName, VisibleAnywhere)
		FText StateSurvivorName;

	UFUNCTION()
		void OnRep_StateSurvivorName();



};
