// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Struct/CWeaponStructures.h"
#include "CDoAction.generated.h"

UCLASS(Blueprintable)
class PROJECTSURVIVAL_API UCDoAction : public UObject
{
	GENERATED_BODY()
	
public:  
	virtual void  BeginPlay
	(
		class ACharacter* InOwner,
		class ACAttachment* InAttachment,
		class UCEquipment* InEquipment
		, const TArray<FDoActionData>& InDoActionData
		//,const TArray<FHitData>& InHitData
	);
	virtual void Tick(float InDeltaTime) {};

public:
	bool GetBeginDoAction() { return bBeginAction; }
	bool GetInAction() { return bInAction; }

	virtual void DoAction();    
	virtual void Begin_DoAction();
	virtual void End_DoAction();

public:
	//장착 관련 
	UFUNCTION()
		virtual void OnBeginEquip() {};
	UFUNCTION()
		virtual void OnUnEquip() {};
public:
	virtual void DestroyWeapon() {};

public:  
	// 콜리젼 관련 
	UFUNCTION()  virtual void OnAttachmentBeginCollision() {};
	UFUNCTION()  virtual void OnAttachmentEndCollision() {};
	UFUNCTION()  virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackerCauser, class ACharacter* InOther) {};
	UFUNCTION()  virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther) {};





protected: 
	UPROPERTY()  
		class ACharacter* OwnerCharacter;
	//UPROPERTY()
	//	class UCMovementComponent* Movement;
	//UPROPERTY()
	//	class UCStateComponent* State;
	UPROPERTY()
		class UWorld* World;

protected:
	TArray<FDoActionData> DoActionDatas;
	//TArray<FHitData>      HitDatas;
protected:
	bool  bInAction = false;   // AI에서 DoAction상태
	bool  bBeginAction = false;   // AI에서 DoAction상태





};
