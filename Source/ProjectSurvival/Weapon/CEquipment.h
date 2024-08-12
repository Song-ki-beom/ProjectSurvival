// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Struct/CWeaponStructures.h"
#include "CEquipment.generated.h"


// 델리게이터 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipment_BeginEquip);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipment_EndEquip);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipment_UnEquip);

UCLASS(Blueprintable)
class PROJECTSURVIVAL_API UCEquipment : public UObject
{
	GENERATED_BODY()
	
public: 
	void  BeginPlay(class ACharacter* InOwner, const FEquipmentData& InData);


public:
	bool GetBeginEquip() { return bBeginEquip; }
	const bool* GetEquipped() { return &bEquipped; }
	bool IsEquipped() { return bEquipped; }


	UFUNCTION(BlueprintNativeEvent)
		void Equip();              
	void Equip_Implementation();   
	UFUNCTION(BlueprintNativeEvent)
		void Begin_Equip();              
	void Begin_Equip_Implementation();   
	UFUNCTION(BlueprintNativeEvent)
		void End_Equip();              
	void End_Equip_Implementation();   
	UFUNCTION(BlueprintNativeEvent)
		void UnEquip();             
	void UnEquip_Implementation();   


public: 
	FEquipment_BeginEquip    OnEquipment_BeginEquip;
	FEquipment_EndEquip      OnEquipment_EndEquip;
	FEquipment_UnEquip       OnEquipment_UnEquip;


public:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;
private:
	FEquipmentData Data;            
	//class UCMovementComponent* Movement;
	//class UCStateComponent* State;
	bool  bBeginEquip = false;     
	bool  bEquipped = false;     



};
