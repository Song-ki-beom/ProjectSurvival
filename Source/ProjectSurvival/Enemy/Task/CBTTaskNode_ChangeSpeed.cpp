// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Task/CBTTaskNode_ChangeSpeed.h"
#include "Enemy/CEnemyAIController.h"
#include "Enemy/CEnemy.h"
#include "ActorComponents/CEnemyAIComponent.h"
#include "ActorComponents/CMovingComponent.h"
#include "Enemy/CEnemyAIController.h"
#include "NavigationSystem.h"

UCBTTaskNode_ChangeSpeed::UCBTTaskNode_ChangeSpeed()
{
    NodeName = "ChangeSpeed";
}
EBTNodeResult::Type UCBTTaskNode_ChangeSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    ACEnemyAIController* controller = Cast<ACEnemyAIController>(OwnerComp.GetOwner());
    ACEnemy* Enemy = Cast<ACEnemy>(controller->GetPawn());
    UCEnemyAIComponent* AIComponent = Cast<UCEnemyAIComponent>(Enemy->GetComponentByClass(UCEnemyAIComponent::StaticClass()));
    UCMovingComponent* MovingComponent = Cast<UCMovingComponent>(Enemy->GetComponentByClass(UCMovingComponent::StaticClass()));
    MovingComponent->SetSpeed(Type);

    return EBTNodeResult::Succeeded;
}
void UCBTTaskNode_ChangeSpeed::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

}