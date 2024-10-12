// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponData.generated.h"
/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCWeaponData : public UObject 
{
	GENERATED_BODY()
private:
	friend class UCWeaponAsset;

public:
	void  SetAttachment(class ACAttachment* InAttachment);
	void  SetEquipment(class  UCEquipment* InEquipment);
	void  SetDoAction(class  UCDoAction* InDoAction);
	void  SetSubAction(class  UCSubAction* InSubAction);
	class ACAttachment* GetAttachment() { return Attachment; }
	class UCEquipment* GetEquipment() { return Equipment; }
	class UCDoAction* GetDoAction() { return DoAction; }
	class UCSubAction* GetSubAction() { return SubAction; }

private:
	UPROPERTY()
		class ACAttachment* Attachment;
	UPROPERTY()
		class UCEquipment* Equipment;
	UPROPERTY()
		class UCDoAction* DoAction;
	UPROPERTY()
		class UCSubAction* SubAction;




};