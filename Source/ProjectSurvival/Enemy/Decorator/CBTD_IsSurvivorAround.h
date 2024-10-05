// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "CBTD_IsSurvivorAround.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCBTD_IsSurvivorAround : public UBTDecorator
{
	GENERATED_BODY()
public:
	UCBTD_IsSurvivorAround();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;



private:
	UPROPERTY(EditAnywhere, Category = "BlackBoard")
		FBlackboardKeySelector TargetActorKey;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
		FBlackboardKeySelector EncounterKey;
	//UPROPERTY(EditAnywhere, Category = "Blackboard")
	//float SearchSphereRadius = 300.0f; // 탐색 반지름 설정

};
