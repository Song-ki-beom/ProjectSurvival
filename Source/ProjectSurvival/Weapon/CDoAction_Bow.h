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
	UPROPERTY(EditDefaultsOnly, Category = "Arrow")
		TSubclassOf<class ACArrow> ArrowClass;
public:
	void DoAction()         override;
	void Begin_DoAction()   override;
	void End_DoAction()     override;

	void OnBeginEquip() override;
	void OnUnEquip()    override;
public:
	UFUNCTION() void OnArrowHit(class AActor* InCauser, class ACharacter* InOtherCharacter);
	UFUNCTION() void OnArrowEndPlay(class ACArrow* InDestroyer);
	UFUNCTION() void ShowAttachedArrow();
	UFUNCTION() void HideAttachedArrow();

private: 
	class UPoseableMeshComponent* PoseableMesh; 
	class USkeletalMeshComponent* SkeletalMesh;

private:
	void CreateArrow();  
	int32 GetAttachedArrow();
	
public:
	void End_BowString();   
	class  UCBowAnimInstance* AnimInstance_Bow;
private:
	FVector OriginLocation;        
	bool    bAttachedString = true; 

private:
	float* Bend = nullptr;
	const  bool* bEquipped;    

public:
	void DestroyWeapon() override;

private:
	struct FArrow
	{
		bool bShooting = false;
		class ACArrow* Arrow = nullptr;
	};

	TArray<FArrow> Arrows;


	

};
