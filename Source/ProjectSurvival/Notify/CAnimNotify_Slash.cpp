// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/CAnimNotify_Slash.h"
#include "Weapon/CDoAction.h"
#include "Weapon/CDoAction_Axe.h"
#include "Weapon/CEquipment.h"
#include "Weapon/CAttachment.h"
#include "ActorComponents/CWeaponComponent.h"


FString  UCAnimNotify_Slash::GetNotifyName_Implementation() const
{
	return "Slash";
}

void UCAnimNotify_Slash::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if(MeshComp == nullptr) return;
	if (MeshComp->GetOwner()==nullptr) return;
	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));

	if (weapon == nullptr) return;
	if (weapon->GetDoAction()==nullptr) return;

	UCDoAction_Axe* axe = Cast<UCDoAction_Axe> (weapon->GetDoAction());
	if (axe == nullptr) return;
	axe->Slash();
}