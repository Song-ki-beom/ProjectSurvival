// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/CAnimNotify_DoAction.h"
#include "Weapon/CDoAction.h"
#include "Weapon/CEquipment.h"
#include "Weapon/CAttachment.h"
#include "ActorComponents/CWeaponComponent.h"


FString  UCAnimNotify_DoAction::GetNotifyName_Implementation() const
{
	return "WeaponDoAction";
}

void UCAnimNotify_DoAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if(MeshComp == nullptr) return;
	if (MeshComp->GetOwner()==nullptr) return;
	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));

	if (weapon == nullptr) return;
	if (weapon->GetDoAction()==nullptr) return;

	weapon->GetDoAction()->Begin_DoAction();
}