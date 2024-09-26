// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Task/CBTTaskNode_ClearFocus.h"
#include "Enemy/CEnemyAIController.h"
#include "Enemy/CEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ActorComponents/CEnemyAIComponent.h"
#include "ActorComponents/CMovingComponent.h"
#include "ActorComponents/CStateComponent.h"
#include "Utility/CDebug.h"
#include "Enemy/CEnemyAIController.h"

UCBTTaskNode_ClearFocus::UCBTTaskNode_ClearFocus()
{
    NodeName = "ClearFocus";
    bNotifyTick = true;
}
EBTNodeResult::Type UCBTTaskNode_ClearFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);


    Controller = Cast<ACEnemyAIController>(OwnerComp.GetOwner());
    if (Controller == nullptr)
    {
        CDebug::Print(TEXT("Controller Missing TASK FAIL"));
        return EBTNodeResult::Failed;
    }
    Controller->ClearFocus(EAIFocusPriority::Gameplay);
   
    Enemy = Cast<ACEnemy>(Controller->GetPawn());
    MovingComponent = Cast<UCMovingComponent>(Enemy->GetComponentByClass(UCMovingComponent::StaticClass()));
    if (MovingComponent == nullptr)
    {
        CDebug::Print(TEXT("MovingComponent Missing TASK FAIL"));
        return EBTNodeResult::Failed;
    }

    return EBTNodeResult::Succeeded;
}



EBTNodeResult::Type UCBTTaskNode_ClearFocus::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

    return EBTNodeResult::Succeeded;
}

