// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_ApproachFriend.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCBTTaskNode_ApproachFriend : public UBTTaskNode
{
		GENERATED_BODY()
public:
	UCBTTaskNode_ApproachFriend();
protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
protected:
	UPROPERTY(EditAnywhere, Category = "Approach")
		float AcceptanceDistance = 200;

private:
	class ACEnemyAIController* controller;
	class  ACEnemy* Enemy;
	class UCEnemyAIComponent* AIComponent;
	class  UCMovingComponent* MovingComponent;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
		FBlackboardKeySelector FriendlyTargetActorKey;
};
