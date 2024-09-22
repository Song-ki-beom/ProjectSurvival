// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/CDoAction.h"
#include "GameFramework/Character.h"
#include "ActorComponents/CStateComponent.h"
#include "ActorComponents/CMovingComponent.h"

void UCDoAction::BeginPlay
(
	class ACharacter* InOwner,
	class ACAttachment* InAttachment,
	class UCEquipment* InEquipment, 
	const TArray<FDoActionData>& InDoActionData
	//, const TArray<FHitData>& InHitData
)
{
	OwnerCharacter = InOwner;
	World = OwnerCharacter->GetWorld();
	DoActionDatas = InDoActionData;
	MovingComponent = Cast<UCMovingComponent>(OwnerCharacter->GetComponentByClass(UCMovingComponent::StaticClass()));
	StateComponent = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));
}

void UCDoAction::DoAction()
{
	bInAction = true;
}

void UCDoAction::Begin_DoAction()
{
	
	bBeginAction = true;
	StateComponent->ChangeType(EStateType::Action);
	if (!DoActionDatas[ActionIdx].bCanMove)
		MovingComponent->Stop();
}

void UCDoAction::End_DoAction()
{
	bInAction = false;
	bBeginAction = false;
	StateComponent->ChangeType(EStateType::Idle);
	if (!DoActionDatas[ActionIdx].bCanMove)
		MovingComponent->Move();
	
}