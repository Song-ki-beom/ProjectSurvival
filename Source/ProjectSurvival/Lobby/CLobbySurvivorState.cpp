// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/CLobbySurvivorState.h"
#include "Net/UnrealNetwork.h"


void ACLobbySurvivorState::OnRep_StateSurvivorName()
{
    UE_LOG(LogTemp, Log, TEXT("Player name replicated: %s"), *StateSurvivorName.ToString());
}

void ACLobbySurvivorState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACLobbySurvivorState, StateSurvivorName);
}