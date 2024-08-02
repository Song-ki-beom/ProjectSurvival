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

protected: // ABP한글 테스트 
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Speed;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Direction;

private:
	class ACharacter* OwnerCharacter;
	bool bCastEnded = false; // OwnerCharacter ĳ��Ʈ �Ǳ� �� NativeUpdateAnimation ȣ�� ���� ����
};

