// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_ClearFocus.generated.h"


UCLASS()
class PROJECTSURVIVAL_API UCBTTaskNode_ClearFocus : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCBTTaskNode_ClearFocus();
protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY()
		class ACEnemyAIController* Controller;
	UPROPERTY()
		class ACEnemy* Enemy;
	UPROPERTY()
		class UCEnemyAIComponent* AIComponent;
	UPROPERTY()
		class UCMovingComponent* MovingComponent;

};
