// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/CDoAction.h"
#include "GameFramework/Character.h"
#include "ActorComponents/CStateComponent.h"
#include "ActorComponents/CMovingComponent.h"
#include "ActorComponents/CStatusComponent.h"
#include "Character/CSurvivorController.h"
#include "Utility/CDebug.h"

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
	if (OwnerCharacter)
	{
		OwnerController = Cast<ACSurvivorController>(OwnerCharacter->GetController());

	}
	World = OwnerCharacter->GetWorld();
	DoActionDatas = InDoActionData;
	MovingComponent = Cast<UCMovingComponent>(OwnerCharacter->GetComponentByClass(UCMovingComponent::StaticClass()));
	StateComponent = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));
	StatusComponent = Cast<UCStatusComponent>(OwnerCharacter->GetComponentByClass(UCStatusComponent::StaticClass()));
}

void UCDoAction::DoAction()
{
	if (bInAction || StateComponent->IsActionMode()) return;
	if (!StatusComponent->CanSpendStamina(10.0f)) return;

	if (OwnerCharacter->HasAuthority())
	{
		StatusComponent->ReduceStamina(15.0f);
	}
	bInAction = true;
	if (DoActionDatas.Num() > 0)
		DoActionDatas[ActionIdx].DoAction(OwnerCharacter);

	
}

void UCDoAction::Begin_DoAction()
{
	
	bBeginAction = true;
	StateComponent->ChangeType(EStateType::Action);
	StatusComponent->SuspendStaminaRecover();
	if (!DoActionDatas[ActionIdx].bCanMove)
		MovingComponent->Stop();

}

void UCDoAction::End_DoAction()
{
	bInAction = false;
	bBeginAction = false;
	StateComponent->ChangeType(EStateType::Idle);
	StatusComponent->ProceedStaminaRecover();
	if (!DoActionDatas[ActionIdx].bCanMove)
		MovingComponent->Move();
	
}