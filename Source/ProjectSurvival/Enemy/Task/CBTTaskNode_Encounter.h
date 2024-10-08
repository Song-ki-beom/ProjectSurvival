// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Encounter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCBTTaskNode_Encounter : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCBTTaskNode_Encounter();
protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
private:
	//void OnMontageFinished();

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
		FBlackboardKeySelector EncounterKey;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
		FBlackboardKeySelector TargetKey;
	class ACEnemyAIController* Controller;
	class ACEnemy* OwnerEnemy;
	class UCEnemyAIComponent* AIComponent;
	class UCMovingComponent* MovingComponent;
	class UCStateComponent* StateComponent;
	FTimerHandle TimerHandle_RetriggerableDelay;
};
