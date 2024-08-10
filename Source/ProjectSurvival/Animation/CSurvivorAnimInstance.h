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

private:
	UFUNCTION()
		void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);

protected: // ABP에서 사용할 변수
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Speed;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Direction;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		EWeaponType WeaponType = EWeaponType::Max;


private:
	class ACharacter* OwnerCharacter;
	class UCWeaponComponent* Weapon;
	bool bCastEnded = false; // OwnerCharacter 캐스트 되기 전 NativeUpdateAnimation 호출 방지 변수
};

