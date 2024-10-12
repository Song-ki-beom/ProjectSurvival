// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/CSubAction.h"
#include "GameFramework/Character.h"
#include "ActorComponents/CStateComponent.h"
#include "ActorComponents/CMovingComponent.h"
#include "Components/CapsuleComponent.h"
#include "Weapon/CAttachment.h"
#include "Weapon/CEquipment.h"
#include "Weapon/CDoAction.h"
#include "Net/UnrealNetwork.h"

UCSubAction::UCSubAction()
{

}

void UCSubAction::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCEquipment* InEquipment, UCDoAction* InDoAction)
{
	Owner = InOwner;
	Attachment = InAttachment;
	Equipment = InEquipment;
	DoAction = InDoAction;
	Movement = Cast<UCMovingComponent>(InOwner->GetComponentByClass(UCMovingComponent::StaticClass()));
	State = Cast<UCStateComponent>(InOwner->GetComponentByClass(UCStateComponent::StaticClass()));
}

void UCSubAction::Pressed()
{
	bInAction = true;
}

void UCSubAction::Released()
{
	bInAction = false;
}

//void UCSubAction::BroadcastPressed_Implementation()
//{
//	bInAction = true;
//}
//
//void UCSubAction::RequestPressed_Implementation()
//{
//	BroadcastPressed();
//}
//
//void UCSubAction::BroadcastReleased_Implementation()
//{
//	bInAction = false;
//}
//
//void UCSubAction::RequestReleased_Implementation()
//{
//	BroadcastReleased();
//}
