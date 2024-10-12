// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/CStateComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"

UCStateComponent::UCStateComponent()
{

}


void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());

}

void UCStateComponent::SetIdleMode()
{
	ChangeType(EStateType::Idle);
}


void UCStateComponent::SetEquipMode()
{
	ChangeType(EStateType::Equip);
}

void UCStateComponent::SetCombatMode()
{
	ChangeType(EStateType::Combat);
}

void UCStateComponent::SetHitMode()
{
	ChangeType(EStateType::Hit);
}

void UCStateComponent::SetActionMode()
{
	ChangeType(EStateType::Action);
}

void UCStateComponent::SetDeadMode()
{
	ChangeType(EStateType::Dead);
}

void UCStateComponent::ChangeType(EStateType InType)
{
	
	BroadcastChangeType(InType);

}

void ::UCStateComponent::BroadcastChangeType_Implementation(EStateType InType)
{
	EStateType prevType = Type;
	Type = InType;
	if (OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(prevType, Type);
}

void UCStateComponent::OnSubActionMode()
{
	bInSubActionMode = true;
}
void UCStateComponent::OffSubActionMode()
{
	bInSubActionMode = false;
}
