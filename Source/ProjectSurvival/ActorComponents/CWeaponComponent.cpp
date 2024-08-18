#include "ActorComponents/CWeaponComponent.h"
#include "CWeaponComponent.h"
#include "Weapon/CWeaponAsset.h"
#include "Weapon/CWeaponData.h"
#include "Weapon/CAttachment.h"
#include "Weapon/CEquipment.h"
#include "Weapon/CDoAction.h"
#include "Net/UnrealNetwork.h"
#include "Utility/CDebug.h"
#include "GameFramework/Character.h"



UCWeaponComponent::UCWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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


void UCWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	/*if (!!GetDoAction())
		GetDoAction()->Tick(DeltaTime);*/


}

void UCWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCWeaponComponent, Type);
	DOREPLIFETIME(UCWeaponComponent, PrevType);


}


void UCWeaponComponent::SetUnarmedMode()
{
	//if(!IsIdleMode()) return;   
	/*if (GetEquipment() == nullptr) return;
	
	GetEquipment()->UnEquip();    
	ChangeType(EWeaponType::Max); */
	SetMode(EWeaponType::Max);
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

void UCWeaponComponent::SetMode(EWeaponType InNewType)
{
	if (OwnerCharacter->HasAuthority()) 
	{
		
		if (!Datas[(int32)InNewType]) return;
		if (Datas[(int32)InNewType]->GetEquipment() == nullptr) return;


		ChangeType(InNewType);
		//Broadcast_Equip();

	}

	else
	{
		RequestSetMode(InNewType);
	}
}

void UCWeaponComponent::RequestSetMode_Implementation(EWeaponType InType)
{
	SetMode(InType);
}

void UCWeaponComponent::ChangeType(EWeaponType InType)
{
	PrevType = Type;
	Type = InType;

	if (OnWeaponTypeChanged.IsBound())
		OnWeaponTypeChanged.Broadcast(PrevType, InType);

}



void UCWeaponComponent::Broadcast_Equip_Implementation()
{
	if (Type == PrevType || Type == EWeaponType::Max)  //무기해제       
	{
		Datas[(int32)PrevType]->GetEquipment()->UnEquip();
		return;
	}

		Datas[(int32)Type]->GetEquipment()->Equip();


}

void UCWeaponComponent::OnRef_PrevTypeChanged()
{
	CDebug::Print(TEXT("OnPrevTypeChanged"));
}

void UCWeaponComponent::OnRef_TypeChanged()
{
	CDebug::Print(TEXT("OnTypeChanged"));
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
	if (IsUnarmedMode()) return nullptr;
	if (!Datas[(int32)Type]) return nullptr;
	return Datas[(int32)Type]->GetDoAction();
}

