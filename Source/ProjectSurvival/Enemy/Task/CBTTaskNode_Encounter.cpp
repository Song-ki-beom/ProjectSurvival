// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Task/CBTTaskNode_Encounter.h"
#include "Enemy/CEnemyAIController.h"
#include "Enemy/CEnemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ActorComponents/CEnemyAIComponent.h"
#include "ActorComponents/CMovingComponent.h"
#include "ActorComponents/CStateComponent.h"
#include "Components/BoxComponent.h"
#include "Utility/CDebug.h"
#include "Enemy/CEnemyAIController.h"

UCBTTaskNode_Encounter::UCBTTaskNode_Encounter()
{
    NodeName = "Encounter";
    bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Encounter::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    bool bHasEncountered = OwnerComp.GetBlackboardComponent()->GetValueAsBool(EncounterKey.SelectedKeyName);
    if (bHasEncountered) return  EBTNodeResult::Succeeded;
        
    
    //OwnerComp.GetBlackboardComponent()->SetValueAsBool(EncounterKey.SelectedKeyName, true);

    UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName);
    if (TargetObject == nullptr) return  EBTNodeResult::Failed;
    AActor* TargetActor = Cast<AActor>(TargetObject);


    Controller = Cast<ACEnemyAIController>(OwnerComp.GetOwner());
    if (Controller == nullptr) return EBTNodeResult::Failed;
    OwnerEnemy = Cast<ACEnemy>(Controller->GetPawn());

    FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(OwnerEnemy->GetActorLocation(), TargetActor->GetActorLocation());

    TargetRot.Pitch = OwnerEnemy->GetActorRotation().Pitch;
    TargetRot.Roll = OwnerEnemy->GetActorRotation().Roll;

    OwnerEnemy->SetActorRotation(TargetRot);

    OwnerEnemy->DoEncounter();

    return EBTNodeResult::InProgress;
}



EBTNodeResult::Type UCBTTaskNode_Encounter::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

    return EBTNodeResult::Succeeded;
}


void UCBTTaskNode_Encounter::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
    bool bHasEncountered = OwnerComp.GetBlackboardComponent()->GetValueAsBool(EncounterKey.SelectedKeyName);

    if (bHasEncountered)
    {
        CDebug::Print(TEXT("Encounter Ended"));
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); //FinishLatentTask : ExecuteTask 에 대해서 InProgress 인 작업을 지연 종료 시킴  
        return;
    }

}