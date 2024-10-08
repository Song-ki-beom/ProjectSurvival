// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Service/CBTService_Bear_Friendly.h"
#include "GameFramework/Character.h"
#include "Enemy/CEnemyAIController.h"
#include "Enemy/CEnemy.h"
#include "Utility/CDebug.h"
#include "ActorComponents/CStateComponent.h"
#include "ActorComponents/CEnemyAIComponent.h"
#include "ActorComponents/CStatusComponent.h"


UCBTService_Bear_Friendly::UCBTService_Bear_Friendly()
{
    this->NodeName = "Bear_Service_Friendly";
    this->Interval = 0.01f;
    this->RandomDeviation = 0.0f;
}

void UCBTService_Bear_Friendly::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    ACEnemyAIController* controller = Cast<ACEnemyAIController>(OwnerComp.GetOwner());
    ACEnemy* Enemy = Cast<ACEnemy>(controller->GetPawn());

    UCStateComponent* state = Cast<UCStateComponent>(Enemy->GetComponentByClass(UCStateComponent::StaticClass()));
    UCStatusComponent* status = Cast<UCStatusComponent>(Enemy->GetComponentByClass(UCStatusComponent::StaticClass()));

    UCEnemyAIComponent* AIComponent = Cast<UCEnemyAIComponent>(Enemy->GetComponentByClass(UCEnemyAIComponent::StaticClass()));

    if (state->IsHitMode())
    {
        AIComponent->SetHittedMode();
        return;
    }

   
    ACharacter* target = AIComponent->GetTarget(); // 적 발견 시 
    if (target == nullptr)
    {
        AIComponent->SetFollowingMode();
        return;
    }
    else if (state->IsIdleMode())
    {
        state->ChangeType(EStateType::Combat);
    };


    float  distance = Enemy->GetDistanceTo(target);
    if (distance < ActionRange)
    {
        AIComponent->SetActionMode();
        return;
    }
    if (target != nullptr)
    {
        AIComponent->SetApproachMode();
        return;
    }


}