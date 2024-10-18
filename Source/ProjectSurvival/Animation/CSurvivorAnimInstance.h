// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ActorComponents/CWeaponComponent.h"
#include "CSurvivorAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCSurvivorAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(Server, Reliable)
		void RequestSetPitch(float InNewPitch);
	UFUNCTION()
		void SetPitch(float InNewPitch);


private:
	UFUNCTION()
		void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);
	UFUNCTION()
		void OnRep_WeaponTypeChanged();
protected: // ABP에서 사용할 변수
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Speed;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Direction;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FRotator Rotation;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Replicated, ReplicatedUsing = OnRep_WeaponTypeChanged)
		EWeaponType WeaponType = EWeaponType::Max;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float  Pitch;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool bFalling;
	UPROPERTY(Replicated,BlueprintReadOnly, EditAnywhere)
		bool bBow_Aiming;
	
	

private:
	UPROPERTY()
		class ACharacter* OwnerCharacter;
	UPROPERTY()
		class UCWeaponComponent* Weapon;

	bool bCastEnded = false; // OwnerCharacter 캐스트 되기 전 NativeUpdateAnimation 호출 방지 변수
};

