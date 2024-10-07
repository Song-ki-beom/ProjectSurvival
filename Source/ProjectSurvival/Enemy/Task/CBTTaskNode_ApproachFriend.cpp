// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Task/CBTTaskNode_ApproachFriend.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/CEnemyAIController.h"
#include "Enemy/CEnemy.h"
#include "DrawDebugHelpers.h"
#include "ActorComponents/CEnemyAIComponent.h"
#include "ActorComponents/CMovingComponent.h"
#include "Enemy/CEnemyAIController.h"
#include "NavigationSystem.h"


UCBTTaskNode_ApproachFriend::UCBTTaskNode_ApproachFriend()
{
    NodeName = "ApproachFriend";
    bNotifyTick = true;
}


EBTNodeResult::Type UCBTTaskNode_ApproachFriend::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    controller = Cast<ACEnemyAIController>(OwnerComp.GetOwner());
    Enemy = Cast<ACEnemy>(controller->GetPawn());
    AIComponent = Cast<UCEnemyAIComponent>(Enemy->GetComponentByClass(UCEnemyAIComponent::StaticClass()));

    return  EBTNodeResult::InProgress;
}

EBTNodeResult::Type UCBTTaskNode_ApproachFriend::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::AbortTask(OwnerComp, NodeMemory);
    //Enemy->End_DoAction();
    return EBTNodeResult::Failed;

}

void UCBTTaskNode_ApproachFriend::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


    UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(FriendlyTargetActorKey.SelectedKeyName);
    if (TargetObject == nullptr) return;
    AActor* TargetActor = Cast<AActor>(TargetObject);
    if (TargetActor == nullptr) return;
    FVector Targetlocation = TargetActor->GetActorLocation();

    EPathFollowingRequestResult::Type result = controller->MoveToLocation(Targetlocation, AcceptanceDistance, true);

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
