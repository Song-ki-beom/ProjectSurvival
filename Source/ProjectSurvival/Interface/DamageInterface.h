// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Struct/CWeaponStructures.h"
#include "DamageInterface.generated.h"




// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageInterface : public UInterface
{
	GENERATED_BODY()
};


//현재 TakeDamage 기본 함수로 인해 사용할 필요가 없어짐 
class ACharacter;
class PROJECTSURVIVAL_API IDamageInterface
{
	GENERATED_BODY()



public:
	virtual void Damage(FDamageData* DamageData)=0;
	
	 


};
