// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Task/CBTTaskNode_Roam.h"
#include "Enemy/CEnemyAIController.h"
#include "Enemy/CEnemy.h"
#include "DrawDebugHelpers.h"
#include "ActorComponents/CEnemyAIComponent.h"
#include "Enemy/CEnemyAIController.h"
#include "NavigationSystem.h"


UCBTTaskNode_Roam::UCBTTaskNode_Roam()
{
    NodeName = "Roam";
    bNotifyTick = true;  
}

EBTNodeResult::Type UCBTTaskNode_Roam::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    ACEnemyAIController* controller = Cast<ACEnemyAIController>(OwnerComp.GetOwner());
    ACEnemy* Enemy = Cast<ACEnemy>(controller->GetPawn());

    UCEnemyAIComponent* AIComponent = Cast<UCEnemyAIComponent>(Enemy->GetComponentByClass(UCEnemyAIComponent::StaticClass()));

   

    UNavigationSystemV1* naviSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(Enemy->GetWorld());
    if (naviSystem == nullptr) return EBTNodeResult::Failed;
    FVector location = Enemy->GetActorLocation();
    FNavLocation point(location);

    while (true)
    {
        if (naviSystem->GetRandomPointInNavigableRadius(location, RandomRadius, point)) //RandomRadius 안에 랜덤 포인트 지점을 잡음
            break;

    }
    AIComponent->SetRoamingLocation(point.Location);
    DrawDebug(Enemy->GetWorld(), point.Location);
    return EBTNodeResult::InProgress;
}


void UCBTTaskNode_Roam::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    ACEnemyAIController* controller = Cast<ACEnemyAIController>(OwnerComp.GetOwner());
    ACEnemy* Enemy = Cast<ACEnemy>(controller->GetPawn());

    UCEnemyAIComponent* AIComponent = Cast<UCEnemyAIComponent>(Enemy->GetComponentByClass(UCEnemyAIComponent::StaticClass()));

    FVector location = AIComponent->GetRoamingLocation();

    EPathFollowingRequestResult::Type result = controller->MoveToLocation(location, AcceptanceDistance, true);





    switch (result)
    {
    case EPathFollowingRequestResult::Failed:
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        break;
    case EPathFollowingRequestResult::AlreadyAtGoal: 
        UNavigationSystemV1* naviSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(Enemy->GetWorld());
        location = Enemy->GetActorLocation();
        FNavLocation point(location);
        while (true)
        {
            if (naviSystem->GetRandomPointInNavigableRadius(location, RandomRadius, point)) //RandomRadius 안에 랜덤 포인트 지점을 잡음
                break;

        }
        AIComponent->SetRoamingLocation(point.Location);
        FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
    break;
    }



    //Move To
 /*   ACharacter* character = AIComponent->GetTarget();
    if(character == nullptr) return;

    float distance = Enemy->GetDistanceTo(character);
    if (distance <= AcceptanceDistance)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }*/


}

void UCBTTaskNode_Roam::DrawDebug(UWorld* InWorld, FVector InLocation)
{
    if (bDebugMode)
        DrawDebugSphere(InWorld, InLocation, 10, 10, FColor::Green, true, 5);
}
