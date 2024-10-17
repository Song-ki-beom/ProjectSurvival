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
    StateComponent = Cast<UCStateComponent>(OwnerEnemy->GetComponentByClass(UCStateComponent::StaticClass()));
    AIComponent = Cast<UCEnemyAIComponent>(OwnerEnemy->GetComponentByClass(UCEnemyAIComponent::StaticClass()));
    FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(OwnerEnemy->GetActorLocation(), TargetActor->GetActorLocation());

    TargetRot.Pitch = OwnerEnemy->GetActorRotation().Pitch;
    TargetRot.Roll = OwnerEnemy->GetActorRotation().Roll;

    OwnerEnemy->SetActorRotation(TargetRot);
    OwnerEnemy->Begin_DoAction();
    OwnerEnemy->DoEncounter();

   
    return EBTNodeResult::InProgress;

}






EBTNodeResult::Type UCBTTaskNode_Encounter::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Task를 즉시 종료하지 않고 몇 초 후에 종료하기 위해 타이머 설정
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle_AbortDelay,
        FTimerDelegate::CreateLambda([this, &OwnerComp]()
            {
                FinishLatentTask(OwnerComp, EBTNodeResult::Aborted); // 몇 초 후에 Task 종료
            }),
        3.0f, // 대기할 시간 (초 단위)
                false
                );

    OwnerEnemy->End_DoAction();
    return EBTNodeResult::InProgress;
}


void UCBTTaskNode_Encounter::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
    if (StateComponent == nullptr) return FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    //CDebug::Print("Bear Current State is:", (int32)StateComponent->GetStateType());
    if (StateComponent->IsIdleMode() || StateComponent->IsCombatMode())
    {
        if (AIComponent)
        {
            AIComponent->GetBlackboard()->SetValueAsBool(FName("bHasEncountered"), true);
            OwnerEnemy->GetMesh()->GetAnimInstance()->Montage_Stop(0.2f);
        }

        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); //FinishLatentTask : ExecuteTask 에 대해서 InProgress 인 작업을 지연 종료 시킴  
        return;
    }

}

//void UCBTTaskNode_Encounter::OnMontageFinished()
//{
//    // 타이머가 종료되면 Task 완료
//    if (Controller)
//    {
//        CDebug::Print("EncounterFinished!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
//        UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(Controller->BrainComponent);
//        FinishLatentTask(*BehaviorTreeComponent, EBTNodeResult::Succeeded);
//    }
//}