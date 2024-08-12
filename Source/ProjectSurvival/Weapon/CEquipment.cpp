// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/CEquipment.h"
#include "GameFramework/Character.h"


void  UCEquipment::BeginPlay(class ACharacter* InOwner, const FEquipmentData& InData)
{
	OwnerCharacter = InOwner;
	Data = InData;

	//Movement = CHelpers::GetComponent<UCMovementComponent>(InOwner);
	//State = CHelpers::GetComponent<UCStateComponent>(InOwner);

}


void UCEquipment::Equip_Implementation()
{
	//State->SetEquipMode();
	/*if (Data.bCanMove == false)
		Movement->Stop();*/
	if (!!Data.Montage)
	{
		OwnerCharacter->PlayAnimMontage(Data.Montage, Data.PlayRate);
		/*UCMontagesComponent* montagesComponent;
		montagesComponent = CHelpers::GetComponent<UCMontagesComponent>(OwnerCharacter);
		if (!!montagesComponent)
			montagesComponent->Montage_Play(Data.Montage, Data.PlayRate);*/
	}
	else // if no Montage 
	{
		Begin_Equip();
		End_Equip();
	}

	/*if (Data.bUseControlRotation)
		Movement->EnableControlRotation();*/


}

void UCEquipment::Begin_Equip_Implementation()
{
	bBeginEquip = true;

	// 노티파이 -> WeaponAsset 에 델리게이트 바인드 -> 바인드된 함수는 DoAction 에 -> DoAction 에서 이벤트 처리 
	if (OnEquipment_BeginEquip.IsBound())
	{
		OnEquipment_BeginEquip.Broadcast();
	}


}

void UCEquipment::End_Equip_Implementation()
{
	bBeginEquip = false;
	bEquipped = true;

	//Movement->Move();    // bCanMove를 true
	//State->SetIdleMode();

	// 노티파이 -> WeaponAsset 에 델리게이트 바인드 -> 바인드된 함수는 DoAction 에 -> DoAction 에서 이벤트 처리 
	if (OnEquipment_EndEquip.IsBound())
		OnEquipment_EndEquip.Broadcast();


}

void UCEquipment::UnEquip_Implementation()
{
	bEquipped = false;

	if (OnEquipment_UnEquip.IsBound())
		OnEquipment_UnEquip.Broadcast();

}

