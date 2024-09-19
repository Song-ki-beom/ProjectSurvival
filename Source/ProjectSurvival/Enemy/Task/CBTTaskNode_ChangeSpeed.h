// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ActorComponents/CMovingComponent.h"
#include "CBTTaskNode_ChangeSpeed.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCBTTaskNode_ChangeSpeed : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCBTTaskNode_ChangeSpeed();
protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:
	UPROPERTY(EditAnywhere, Category = "Type")
		ESpeedType Type;
};
