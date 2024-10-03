// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Animation/AnimMontage.h"
#include "CBTTaskNode_EatFood.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCBTTaskNode_EatFood : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCBTTaskNode_EatFood();
	UFUNCTION()
		void OnEatFoodMontageEnded();


protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


private:
	class ACEnemyAIController* controller;
	class  ACEnemy* Enemy;
	class UCEnemyAIComponent* AIComponent;
	class  UCMovingComponent* MovingComponent;
	class UCMontageComponent* MontageComponent;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
		FBlackboardKeySelector TargetFoodActorKey;
	UPROPERTY(EditAnywhere, Category = "Montage")
		UAnimMontage* EatFoodMontage;

	bool bIsMontageEnded;
	bool bMontageInterrupted;
	FOnMontageEnded EndDelegate;
	UPROPERTY()
	UAnimInstance* AnimInstance;
};
