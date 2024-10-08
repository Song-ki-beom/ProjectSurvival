// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Task/CBTTaskNode_Approach.h"
#include "Enemy/CEnemyAIController.h"
#include "Enemy/CEnemy.h"
#include "DrawDebugHelpers.h"
#include "ActorComponents/CEnemyAIComponent.h"
#include "ActorComponents/CMovingComponent.h"
#include "Enemy/CEnemyAIController.h"
#include "NavigationSystem.h"


UCBTTaskNode_Approach::UCBTTaskNode_Approach()
{
    NodeName = "Approach";
    bNotifyTick = true;
}




EBTNodeResult::Type UCBTTaskNode_Approach::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    controller = Cast<ACEnemyAIController>(OwnerComp.GetOwner());
    Enemy = Cast<ACEnemy>(controller->GetPawn());
    AIComponent = Cast<UCEnemyAIComponent>(Enemy->GetComponentByClass(UCEnemyAIComponent::StaticClass()));
   

    return  EBTNodeResult::InProgress;
}

EBTNodeResult::Type UCBTTaskNode_Approach::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::AbortTask(OwnerComp, NodeMemory);
    //Enemy->End_DoAction();
    return EBTNodeResult::Failed;

}

void UCBTTaskNode_Approach::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

  
    ACharacter* Target = AIComponent->GetTarget();
    if (Target == nullptr) return;
    FVector TargetLocation = Target->GetActorLocation();

 
    //EPathFollowingRequestResult::Type result = controller->MoveToLocation(TargetLocation, AcceptanceDistance, true);
    FNavPathSharedPtr NavPath;
    FAIMoveRequest MoveRequest(TargetLocation);
    controller->ChangeTargetLocation(TargetLocation);

    MoveRequest.SetUsePathfinding(true);
    //MoveRequest.SetAllowPartialPath(true);  // 차단된 경우에도 가능한 경로 사용
    MoveRequest.SetProjectGoalLocation(true);
    MoveRequest.SetAcceptanceRadius(AcceptanceDistance);
    EPathFollowingRequestResult::Type result = controller->MoveTo(MoveRequest, &NavPath);

    switch (result)
    {
    case EPathFollowingRequestResult::Failed:
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        break;
    case EPathFollowingRequestResult::AlreadyAtGoal:
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        break;
    }


}
