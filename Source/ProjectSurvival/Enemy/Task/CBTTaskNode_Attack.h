// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Attack.generated.h"



UCLASS()
class PROJECTSURVIVAL_API UCBTTaskNode_Attack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_Attack();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	//void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	UFUNCTION()
	void OnMontageFinished();


private:
	class ACEnemyAIController* Controller;
	class ACEnemy* Enemy;
	class UCEnemyAIComponent* AIComponent;
	class UCMovingComponent* MovingComponent;
	class UCStateComponent* StateComponent;
	int32 count;
	FTimerHandle TimerHandle_RetriggerableDelay;


};
