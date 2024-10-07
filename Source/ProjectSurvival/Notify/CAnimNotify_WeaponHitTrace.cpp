#include "Notify/CAnimNotify_WeaponHitTrace.h"
#include "Weapon/CDoAction.h"
#include "Weapon/CDoAction_Weapon.h"
#include "Weapon/CEquipment.h"
#include "Weapon/CAttachment.h"
#include "ActorComponents/CWeaponComponent.h"

FString UCAnimNotify_WeaponHitTrace::GetNotifyName_Implementation() const
{
	return "WeaponHitTrace";
}

void UCAnimNotify_WeaponHitTrace::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp == nullptr) return;
	if (MeshComp->GetOwner() == nullptr) return;
	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));

	if (weapon == nullptr) return;
	if (weapon->GetDoAction() == nullptr) return;

	UCDoAction_Weapon* doActionWeapon = Cast<UCDoAction_Weapon>(weapon->GetDoAction());
	if (doActionWeapon == nullptr) return;
	doActionWeapon->WeaponHitTrace();
}