// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Task/CBTTaskNode_Attack.h"
#include "Enemy/CEnemyAIController.h"
#include "Enemy/CEnemy.h"
#include "ActorComponents/CEnemyAIComponent.h"
#include "ActorComponents/CMovingComponent.h"
#include "ActorComponents/CStateComponent.h"
#include "Utility/CDebug.h"
#include "Enemy/CEnemyAIController.h"
#include "Net/UnrealNetwork.h"
#include "NavigationSystem.h"

UCBTTaskNode_Attack::UCBTTaskNode_Attack()
{
    NodeName = "Attack";
    bNotifyTick = true;  
}
EBTNodeResult::Type UCBTTaskNode_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    Controller = Cast<ACEnemyAIController>(OwnerComp.GetOwner());
    if (Controller == nullptr) return EBTNodeResult::Failed;
    Enemy = Cast<ACEnemy>(Controller->GetPawn());
    AIComponent = Cast<UCEnemyAIComponent>(Enemy->GetComponentByClass(UCEnemyAIComponent::StaticClass()));
    ACharacter* Target = AIComponent->GetTarget();
    if (Target)
    {
        Controller->SetFocus(Target);
    }
    Controller->StopMovement();

    Enemy->DoAction();
        
    
   

   
    return EBTNodeResult::InProgress;
}



EBTNodeResult::Type UCBTTaskNode_Attack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::AbortTask(OwnerComp, NodeMemory);
    Enemy->EndDoAction();
    return EBTNodeResult::Succeeded;

}

void UCBTTaskNode_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


    
    UCStateComponent* StateComponent = Cast<UCStateComponent>(Enemy->GetComponentByClass(UCStateComponent::StaticClass()));
    if (StateComponent->IsIdleMode())
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); //FinishLatentTask : ExecuteTask 에 대해서 InProgress 인 작업을 지연 종료 시킴  
        return;
    }

}
