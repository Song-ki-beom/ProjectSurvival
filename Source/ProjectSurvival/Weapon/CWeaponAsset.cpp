// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/CWeaponAsset.h"
#include "GameFramework/Character.h"
#include "Weapon/CAttachment.h"
#include "Weapon/CWeaponData.h"
#include "Weapon/CEquipment.h"
#include "Weapon/CDoAction.h"
#include "Weapon/CSubAction.h"

UCWeaponAsset::UCWeaponAsset()
{
	AttachmentClass = ACAttachment::StaticClass();
	EquipmentClass = UCEquipment::StaticClass();
	DoActionClass = UCDoAction::StaticClass();

}


void UCWeaponAsset::BeginPlay(ACharacter* InOwner, UCWeaponData** OutWeaponData)
{
	ACAttachment* Attachment = nullptr;
	if (!!AttachmentClass) 
	{
		FActorSpawnParameters params;
		params.Owner = InOwner;
		Attachment = InOwner->GetWorld()->SpawnActor<ACAttachment>(AttachmentClass, params);
	}
	UCEquipment* Equipment = nullptr;
	if (!!EquipmentClass)  
	{
		Equipment = NewObject<UCEquipment>(this, EquipmentClass);
		Equipment->BeginPlay(InOwner, EquipmentData);


		if (!!Attachment) 
		{
			Equipment->OnEquipment_BeginEquip.AddDynamic(Attachment, &ACAttachment::OnBeginEquip);
			Equipment->OnEquipment_EndEquip.AddDynamic(Attachment, &ACAttachment::OnEndEquip);
			Equipment->OnEquipment_UnEquip.AddDynamic(Attachment, &ACAttachment::OnUnEquip);
		}
	}

	UCDoAction* DoAction = nullptr;
	if (!!DoActionClass)
	{
		DoAction = NewObject<UCDoAction>(this, DoActionClass);
		DoAction->BeginPlay(InOwner, Attachment, Equipment, DoActionDatas);

		if (!!Equipment)
		{
			Equipment->OnEquipment_BeginEquip.AddDynamic(DoAction, &UCDoAction::OnBeginEquip);
			Equipment->OnEquipment_UnEquip.AddDynamic(DoAction, &UCDoAction::OnUnEquip);

		}
	}


	// SubAction
	UCSubAction* SubAction = nullptr;
	if (!!SubActionClass)
	{
		SubAction = NewObject<UCSubAction>(this, SubActionClass);
		SubAction->BeginPlay(InOwner, Attachment, Equipment, DoAction);
	}


	//WeaponData에 동적 할당 
	*OutWeaponData = NewObject<UCWeaponData>();
	(*OutWeaponData)->SetAttachment(Attachment);
	(*OutWeaponData)->SetEquipment(Equipment);
	(*OutWeaponData)->SetDoAction(DoAction);
	(*OutWeaponData)->SetSubAction(SubAction);

}


