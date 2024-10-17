// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Task/CBTTaskNode_RunAway.h"
#include "Enemy/CEnemyAIController.h"
#include "Enemy/CEnemy.h"
#include "DrawDebugHelpers.h"
#include "ActorComponents/CEnemyAIComponent.h"
#include "ActorComponents/CMovingComponent.h"
#include "Enemy/CEnemyAIController.h"
#include "NavigationSystem.h"


UCBTTaskNode_RunAway::UCBTTaskNode_RunAway()
{
    NodeName = "RunAway";
    bNotifyTick = true;
}




EBTNodeResult::Type UCBTTaskNode_RunAway::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    controller = Cast<ACEnemyAIController>(OwnerComp.GetOwner());
    Enemy = Cast<ACEnemy>(controller->GetPawn());
    AIComponent = Cast<UCEnemyAIComponent>(Enemy->GetComponentByClass(UCEnemyAIComponent::StaticClass()));



    ACharacter* Target = AIComponent->GetTarget();
    if (Target == nullptr) return EBTNodeResult::Failed;
    FVector RunAwayDirection = Enemy->GetActorLocation() - Target->GetActorLocation();
    RunAwayDirection = RunAwayDirection.GetSafeNormal();
    FVector RunAwayLocation = Enemy->GetActorLocation() + (RunAwayDirection*RunAwayAmount*1.3f);
    UNavigationSystemV1* naviSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(Enemy->GetWorld());
    if (naviSystem == nullptr) return EBTNodeResult::Failed;
    FNavLocation point(RunAwayLocation);

    const int32 MaxAttempts = 10;  // 최대 시도 횟수
    int32 AttemptCnt = 0;

    while (AttemptCnt < MaxAttempts)
    {
        if (naviSystem->GetRandomPointInNavigableRadius(RunAwayLocation, RandomRadius, point)) //RandomRadius 안에 랜덤 포인트 지점을 잡음
            break;
        AttemptCnt++;
    }

    if (AttemptCnt >= MaxAttempts)
    {
        return EBTNodeResult::Failed;
    }

    AIComponent->SetRoamingLocation(point.Location);
    //DrawDebug(Enemy->GetWorld(), point.Location);

    return  EBTNodeResult::InProgress;



}

EBTNodeResult::Type UCBTTaskNode_RunAway::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::AbortTask(OwnerComp, NodeMemory);
    //Enemy->End_DoAction();
    controller->StopMovement();
    return EBTNodeResult::Failed;

}

void UCBTTaskNode_RunAway::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


   if(controller == nullptr) FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    if (Enemy == nullptr) FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    if (AIComponent == nullptr) FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

    FVector location = AIComponent->GetRoamingLocation();

    EPathFollowingRequestResult::Type result = controller->MoveToLocation(location, 50.0f, true);


    switch (result)
    {
    case EPathFollowingRequestResult::Failed:
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        break;
    case EPathFollowingRequestResult::AlreadyAtGoal:
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        break;
    }

}


void UCBTTaskNode_RunAway::DrawDebug(UWorld* InWorld, FVector InLocation)
{
    //if (bDebugMode)
        //DrawDebugSphere(InWorld, InLocation, 10, 10, FColor::Green, true, 5);
}

