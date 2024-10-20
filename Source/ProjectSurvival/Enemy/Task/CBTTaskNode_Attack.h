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
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	//UFUNCTION()
	//void OnMontageFinished();


private:
	UPROPERTY()
		class ACEnemyAIController* Controller;
	UPROPERTY()
		class ACEnemy* Enemy;
	UPROPERTY()
		class UCEnemyAIComponent* AIComponent;
	UPROPERTY()
		class UCMovingComponent* MovingComponent;
	UPROPERTY()
		class UCStateComponent* StateComponent;
	int32 count;
	FTimerHandle TimerHandle_RetriggerableDelay;


};
