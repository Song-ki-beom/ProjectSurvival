// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/CDoAction.h"
#include "CDoAction_Bow.generated.h"


UCLASS()
class PROJECTSURVIVAL_API UCDoAction_Bow : public UCDoAction
{
	GENERATED_BODY()
		virtual void  BeginPlay
		(
			class ACharacter* InOwner,
			class ACAttachment* InAttachment,
			class UCEquipment* InEquipment,
			const TArray<FDoActionData>& InDoActionData
		) override;
	virtual void Tick(float InDeltaTime) override;

public:
	void DoAction()         override;
	void Begin_DoAction()   override;
	void End_DoAction()     override;

	void OnBeginEquip() override;
	void OnUnEquip()    override;

private: 
	class UPoseableMeshComponent* PoseableMesh; 
	class USkeletalMeshComponent* SkeletalMesh;
	class UCAmmoComponent* AmmoComponent;
public:
	void End_BowString();   
	class  UCBowAnimInstance* AnimInstance_Bow;
private:
	FVector OriginLocation;        
	bool    bAttachedString = true; 

	float* Bend = nullptr;
	const  bool* bEquipped;    

public:
	void DestroyWeapon() override;


public:
	UPROPERTY(EditDefaultsOnly, Category = "Arrow")
		TSubclassOf<class ACArrow> ArrowClass;
	

};
