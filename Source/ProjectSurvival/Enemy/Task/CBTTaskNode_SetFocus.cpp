// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Task/CBTTaskNode_SetFocus.h"
#include "Enemy/CEnemyAIController.h"
#include "Enemy/CEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ActorComponents/CEnemyAIComponent.h"
#include "ActorComponents/CMovingComponent.h"
#include "ActorComponents/CStateComponent.h"
#include "Components/BoxComponent.h"
#include "Utility/CDebug.h"
#include "Enemy/CEnemyAIController.h"

UCBTTaskNode_SetFocus::UCBTTaskNode_SetFocus()
{
    NodeName = "SetFocus";
    bNotifyTick = true;
}
EBTNodeResult::Type UCBTTaskNode_SetFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(Target.SelectedKeyName);
    if (TargetObject == nullptr) return  EBTNodeResult::Failed;
    AActor* TargetActor = Cast<AActor>(TargetObject);
   


    Controller = Cast<ACEnemyAIController>(OwnerComp.GetOwner());
    if (Controller == nullptr) return EBTNodeResult::Failed;
    Controller->SetFocus(TargetActor);

    Enemy = Cast<ACEnemy>(Controller->GetPawn());
    MovingComponent = Cast<UCMovingComponent>(Enemy->GetComponentByClass(UCMovingComponent::StaticClass()));
    if (MovingComponent == nullptr) return EBTNodeResult::Failed;



    return EBTNodeResult::Succeeded;
}



EBTNodeResult::Type UCBTTaskNode_SetFocus::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
  
    return EBTNodeResult::Succeeded;
}


