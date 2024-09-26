// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ActorComponents/CStateComponent.h"
#include "CEnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	void NativeInitializeAnimation() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UFUNCTION()
	void OnStateTypeChangedHandler(EStateType prevType, EStateType NewType);


protected: // ABP에서 사용할 변수
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Speed;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Direction;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		EStateType StateType = EStateType::Idle;


private:
	
	//참조용 
	class ACharacter* OwnerCharacter;
	class UCStateComponent* StateComponent;
	bool bCastEnded = false; // OwnerCharacter 캐스트 되기 전 NativeUpdateAnimation 호출 방지 변수
	FRotator PrevRotation;

};
