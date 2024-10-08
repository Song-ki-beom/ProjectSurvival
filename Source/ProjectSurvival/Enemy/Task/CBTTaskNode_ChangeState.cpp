// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Task/CBTTaskNode_ChangeState.h"
#include "Enemy/CEnemyAIController.h"
#include "Enemy/CEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ActorComponents/CMovingComponent.h"
#include "Enemy/CEnemyAIController.h"
#include "NavigationSystem.h"

UCBTTaskNode_ChangeState::UCBTTaskNode_ChangeState()
{
    NodeName = "ChangeState";
}
EBTNodeResult::Type UCBTTaskNode_ChangeState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    ACEnemyAIController* controller = Cast<ACEnemyAIController>(OwnerComp.GetOwner());
    ACEnemy* Enemy = Cast<ACEnemy>(controller->GetPawn());
    UCEnemyAIComponent* AIComponent = Cast<UCEnemyAIComponent>(Enemy->GetComponentByClass(UCEnemyAIComponent::StaticClass()));
    UCMovingComponent* MovingComponent = Cast<UCMovingComponent>(Enemy->GetComponentByClass(UCMovingComponent::StaticClass()));

    AIComponent->ChangeAIStateType(TargetType);

    return EBTNodeResult::Succeeded;
}
void UCBTTaskNode_ChangeState::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

}