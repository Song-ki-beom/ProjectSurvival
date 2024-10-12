// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/CAnimNotify_EndBowString.h"
#include "Weapon/CDoAction.h"
#include "Weapon/CEquipment.h"
#include "Weapon/CAttachment.h"
#include "ActorComponents/CWeaponComponent.h"
#include "Weapon/CDoAction_Bow.h"

FString  UCAnimNotify_EndBowString::GetNotifyName_Implementation() const
{
	return "End_BowString";
}
void UCAnimNotify_EndBowString::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if(MeshComp == nullptr) return;
	if (MeshComp->GetOwner() == nullptr) return;

	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));
	if(weapon == nullptr) return;
	if(weapon->GetDoAction() == nullptr) return;

	UCDoAction_Bow* bow = Cast<UCDoAction_Bow>(weapon->GetDoAction());
	if(bow == nullptr) return;
	bow->End_BowString();
}
