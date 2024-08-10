#include "ActorComponents/CWeaponComponent.h"
//#include "Components/CStateComponent.h"
#include "CWeaponComponent.h"
#include "Weapon/CWeaponAsset.h"
#include "Weapon/CWeaponData.h"
#include "Weapon/CAttachment.h"
#include "Weapon/CEquipment.h"
#include "Weapon/CDoAction.h"
#include "GameFramework/Character.h"



UCWeaponComponent::UCWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	for (int32 i = 0; i < (int32)EWeaponType::Max; i++)
	{
		if (!!DataAssets[i])
			DataAssets[i]->BeginPlay(OwnerCharacter, &Datas[i]);
		else
			Datas[i] = nullptr;
	}
	
}


// Called every frame
void UCWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	/*
	if (!!GetDoAction())
		GetDoAction()->Tick(DeltaTime);*/


}

void UCWeaponComponent::SetUnarmedMode()
{
	//if(!IsIdleMode()) return;   
	if (GetEquipment() == nullptr) return;
	
	GetEquipment()->UnEquip();    
	ChangeType(EWeaponType::Max); 
}

void UCWeaponComponent::SetAxeMode()
{
	//if(!IsIdleMode()) return;
	SetMode(EWeaponType::Axe);
}

bool UCWeaponComponent::IsUnarmedMode()
{
	 return Type == EWeaponType::Max; 
}

bool UCWeaponComponent::IsAxeMode()
{
	return Type == EWeaponType::Axe;
}

bool UCWeaponComponent::IsIdleMode()
{
	//return Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));
	 return true;

}

void UCWeaponComponent::DoAction()
{
	if (!!GetDoAction())
		GetDoAction()->DoAction();
}

void UCWeaponComponent::DestroyWeapon()
{
	for (int32 i = 0; i < (int32)EWeaponType::Max; i++)
	{
		if (!!Datas[i] && Datas[i]->GetAttachment())
			Datas[i]->GetAttachment()->Destroy();
		if (!!Datas[i] && Datas[i]->GetDoAction())
			Datas[i]->GetDoAction()->DestroyWeapon();


	}
}

void UCWeaponComponent::SetMode(EWeaponType InType)
{
	if (Type == InType)  //같은 무기 두번 Press -> 무기 해제        
	{
		SetUnarmedMode();
		return;
	}

	
	if(!Datas[(int32)InType]) return;
	if (Datas[(int32)InType]->GetEquipment() ==nullptr) return;

	Datas[(int32)InType]->GetEquipment()->Equip();   
	ChangeType(InType);

}

void UCWeaponComponent::ChangeType(EWeaponType InType)
{
	EWeaponType prevTYpe = Type;
	Type = InType;

	if (OnWeaponTypeChanged.IsBound())
		OnWeaponTypeChanged.Broadcast(prevTYpe, InType);
}

ACAttachment* UCWeaponComponent::GetAttachment()
{
	if (IsUnarmedMode()) return nullptr;
	if(!Datas[(int32)Type]) return nullptr;
	return Datas[(int32)Type]->GetAttachment();
}


UCEquipment* UCWeaponComponent::GetEquipment()
{
	if (IsUnarmedMode()) return nullptr;
	if (!Datas[(int32)Type]) return nullptr;
	return Datas[(int32)Type]->GetEquipment();
}

UCDoAction* UCWeaponComponent::GetDoAction()
{
	if (!IsUnarmedMode()) return nullptr;
	if (!Datas[(int32)Type]) return nullptr;
	return Datas[(int32)Type]->GetDoAction();
}

