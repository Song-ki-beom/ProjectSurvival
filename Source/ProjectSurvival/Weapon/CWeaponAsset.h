// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Struct/CWeaponStructures.h"
#include "CWeaponAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCWeaponAsset : public UDataAsset
{
	GENERATED_BODY()
	
public: 
	UCWeaponAsset();
public:  
	void  BeginPlay(class ACharacter* InOwner, class UCWeaponData** OutWeaponData);
private:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACAttachment> AttachmentClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCEquipment>  EquipmentClass; 
	UPROPERTY(EditAnywhere)
		FEquipmentData  EquipmentData;                   
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCDoAction>  DoActionClass;    

	UPROPERTY(EditAnywhere)
		TArray<FDoActionData> DoActionDatas;        
	
	//UPROPERTY(EditAnywhere) 
	//TArray<FHitData> HitDatas;        
};
