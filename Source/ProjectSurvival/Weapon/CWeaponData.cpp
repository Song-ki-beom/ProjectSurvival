// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/CWeaponData.h"
#include "Weapon/CAttachment.h"
#include "Weapon/CEquipment.h"
#include "Weapon/CDoAction.h"

void  UCWeaponData::SetAttachment(class ACAttachment* InAttachment)
{
	Attachment = InAttachment;
}

void UCWeaponData::SetEquipment(UCEquipment* InEquipment)
{
	Equipment = InEquipment;
}

void UCWeaponData::SetDoAction(UCDoAction* InDoAction)
{
	DoAction = InDoAction;
}

