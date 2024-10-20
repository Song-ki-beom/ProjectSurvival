// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_RunAway.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCBTTaskNode_RunAway : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCBTTaskNode_RunAway();
protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:
	void  DrawDebug(UWorld* InWorld, FVector InLocation);

protected:
	UPROPERTY(EditAnywhere, Category = "Patrol")
		bool bDebugMode = true;

	UPROPERTY(EditAnywhere, Category = "RunAway")
		float RandomRadius = 1200;

	UPROPERTY(EditAnywhere, Category = "RunAway")
		float RunAwayAmount = 2000;

private:
	UPROPERTY()
		class ACEnemyAIController* controller;
	UPROPERTY()
		class  ACEnemy* Enemy;
	UPROPERTY()
		class UCEnemyAIComponent* AIComponent;
	UPROPERTY()
		class  UCMovingComponent* MovingComponent;



};
