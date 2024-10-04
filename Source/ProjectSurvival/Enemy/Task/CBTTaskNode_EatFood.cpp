// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Task/CBTTaskNode_EatFood.h"
#include "World/CPickUp.h"
#include "Utility/CDebug.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/CEnemyAIController.h"
#include "Enemy/CEnemy.h"
#include "DrawDebugHelpers.h"
#include "ActorComponents/CEnemyAIComponent.h"
#include "ActorComponents/CMovingComponent.h"
#include "ActorComponents/CMontageComponent.h"
#include "Enemy/CEnemyAIController.h"
#include "NavigationSystem.h"


UCBTTaskNode_EatFood::UCBTTaskNode_EatFood()
{
    NodeName = "EatFood";
    bNotifyTick = true;
}


EBTNodeResult::Type UCBTTaskNode_EatFood::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    controller = Cast<ACEnemyAIController>(OwnerComp.GetOwner());
    Enemy = Cast<ACEnemy>(controller->GetPawn());
    AIComponent = Cast<UCEnemyAIComponent>(Enemy->GetComponentByClass(UCEnemyAIComponent::StaticClass()));
    MontageComponent = Cast<UCMontageComponent>(Enemy->GetComponentByClass(UCMontageComponent::StaticClass()));

    bIsMontageEnded = false;
    bMontageInterrupted = false;

    UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetFoodActorKey.SelectedKeyName);
    if (TargetObject == nullptr) return EBTNodeResult::Failed;
    TargetPickUp = Cast<ACPickUp>(TargetObject);
    if (TargetPickUp == nullptr) return EBTNodeResult::Failed;

    if (EatFoodMontage)
    {
        AnimInstance = Enemy->GetMesh()->GetAnimInstance();
        if (AnimInstance && MontageComponent)
        {
            MontageComponent->BindFoodMontageEnded();
            if(!MontageComponent->OnFoodMontageNotifyEnd.IsBound())
                MontageComponent->OnFoodMontageNotifyEnd.AddDynamic(this, &UCBTTaskNode_EatFood::OnEatFoodMontageEnded);
            AnimInstance->Montage_Play(EatFoodMontage , 1.5f);
        }
    }

 
    
    return  EBTNodeResult::InProgress;
}

EBTNodeResult::Type UCBTTaskNode_EatFood::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::AbortTask(OwnerComp, NodeMemory);
    //Enemy->End_DoAction();
    return EBTNodeResult::Failed;

}

void UCBTTaskNode_EatFood::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    if (bIsMontageEnded) 
            FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);

}


void UCBTTaskNode_EatFood::OnEatFoodMontageEnded()
{

    Enemy->EatFood(TargetPickUp);
    bIsMontageEnded = true;

}