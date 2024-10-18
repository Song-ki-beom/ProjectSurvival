// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Task/CTTaskNode_StopMove.h"
#include "Enemy/CEnemyAIController.h"
#include "Enemy/CEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ActorComponents/CMovingComponent.h"
#include "Enemy/CEnemyAIController.h"
#include "NavigationSystem.h"

UCTTaskNode_StopMove::UCTTaskNode_StopMove()
{
    NodeName = "StopMove";
}
EBTNodeResult::Type UCTTaskNode_StopMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    ACEnemyAIController* controller = Cast<ACEnemyAIController>(OwnerComp.GetOwner());
    if (controller == nullptr) return EBTNodeResult::Succeeded;
    controller->StopMovement();

    return EBTNodeResult::Succeeded;
}
