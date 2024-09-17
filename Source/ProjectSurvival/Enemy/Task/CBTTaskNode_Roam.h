// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Roam.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCBTTaskNode_Roam : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCBTTaskNode_Roam();
protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Patrol")
		bool bDebugMode = true;
	UPROPERTY(EditAnywhere, Category = "Patrol")
		float AcceptanceDistance = 50;
	UPROPERTY(EditAnywhere, Category = "Patrol")
		float RandomRadius = 20;

private:
	void  DrawDebug(UWorld* InWorld, FVector InLocation);
};
