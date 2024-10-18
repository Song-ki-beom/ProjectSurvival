// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CTTaskNode_StopMove.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCTTaskNode_StopMove : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCTTaskNode_StopMove();
protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	

};
