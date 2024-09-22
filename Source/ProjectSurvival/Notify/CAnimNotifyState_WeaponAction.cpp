// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/CAnimNotifyState_WeaponAction.h"
#include "Weapon/CDoAction.h"
#include "ActorComponents/CWeaponComponent.h"
FString  UCAnimNotifyState_WeaponAction::GetNotifyName_Implementation() const
{
	return "WeaponActionState";
}

void UCAnimNotifyState_WeaponAction::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	if (MeshComp == nullptr) return;
	if (MeshComp->GetOwner() == nullptr) return;
	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));

	if (weapon == nullptr) return;
	if (weapon->GetDoAction() == nullptr) return;
	weapon->GetDoAction()->Begin_DoAction();


}
void UCAnimNotifyState_WeaponAction::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (MeshComp == nullptr) return;
	if (MeshComp->GetOwner() == nullptr) return;

	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));

	if (weapon == nullptr) return;
	if (weapon->GetDoAction() == nullptr) return;
	weapon->GetDoAction()->End_DoAction();


}