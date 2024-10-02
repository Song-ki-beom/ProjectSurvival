// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "CBTD_IsFoodAround.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCBTD_IsFoodAround : public UBTDecorator
{
	GENERATED_BODY()

public:
	UCBTD_IsFoodAround();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
		FBlackboardKeySelector TargetFoodActorKey;
};



