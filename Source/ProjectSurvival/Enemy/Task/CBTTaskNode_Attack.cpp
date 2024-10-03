// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Task/CBTTaskNode_Attack.h"
#include "Enemy/CEnemyAIController.h"
#include "Enemy/CEnemy.h"
#include "ActorComponents/CEnemyAIComponent.h"
#include "ActorComponents/CMovingComponent.h"
#include "ActorComponents/CStateComponent.h"
#include "Utility/CDebug.h"
#include "Enemy/CEnemyAIController.h"

UCBTTaskNode_Attack::UCBTTaskNode_Attack()
{
    NodeName = "Attack";
    bNotifyTick = true;  
}
EBTNodeResult::Type UCBTTaskNode_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    Controller = Cast<ACEnemyAIController>(OwnerComp.GetAIOwner());
    if (Controller == nullptr) return EBTNodeResult::Succeeded;
    Enemy = Cast<ACEnemy>(Controller->GetPawn());


    if (Enemy)
    {
        StateComponent = Cast<UCStateComponent>(Enemy->GetComponentByClass(UCStateComponent::StaticClass()));
        if (StateComponent == nullptr) return EBTNodeResult::Succeeded;
        StateComponent->ChangeType(EStateType::Action);
        Controller->StopMovement();
        Enemy->DoAction();
        const float MontageDuration = Enemy->DoAction();
        if (MontageDuration > 0.0f)
        {
            GetWorld()->GetTimerManager().SetTimer(TimerHandle_RetriggerableDelay, this, &UCBTTaskNode_Attack::OnMontageFinished, MontageDuration, false);
            return EBTNodeResult::InProgress;
        }
    }
    return EBTNodeResult::Failed;
    
}



EBTNodeResult::Type UCBTTaskNode_Attack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::AbortTask(OwnerComp, NodeMemory);
    Enemy->End_DoAction();
    GetWorld()->GetTimerManager().ClearTimer(TimerHandle_RetriggerableDelay);
    return EBTNodeResult::Failed;

}

//void UCBTTaskNode_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
//{
//    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
//
//    if (StateComponent == nullptr) return FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
//
//    if (StateComponent->IsIdleMode() || StateComponent->IsCombatMode())
//    {
//        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); //FinishLatentTask : ExecuteTask 에 대해서 InProgress 인 작업을 지연 종료 시킴  
//        return;
//    }
//
//}

void UCBTTaskNode_Attack::OnMontageFinished()
{
    // 타이머가 종료되면 Task 완료
    if (Controller)
    {
        UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(Controller->BrainComponent);
        FinishLatentTask(*BehaviorTreeComponent, EBTNodeResult::Succeeded);
    }
}