// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
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

protected: // ABP에서 사용할 변수
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Speed;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Direction;

private:
	class ACharacter* OwnerCharacter;
	bool bCastEnded = false; // OwnerCharacter 캐스트 되기 전 NativeUpdateAnimation 호출 방지 변수
};

