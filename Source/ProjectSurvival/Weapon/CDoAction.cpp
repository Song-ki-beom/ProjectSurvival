// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/CDoAction.h"
#include "GameFramework/Character.h"
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

}

void UCDoAction::DoAction()
{
	bInAction = true;
	//State->SetActionMode();
}

void UCDoAction::Begin_DoAction()
{
	bBeginAction = true;
}

void UCDoAction::End_DoAction()
{
	bInAction = false;
	bBeginAction = false;

	//State->SetIdleMode();
	//Movement->Move();
	//Movement->DisableFixedCamera();
}