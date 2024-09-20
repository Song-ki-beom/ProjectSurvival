// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Service/CBTService_Bear.h"

// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Character.h"
#include "Enemy/CEnemyAIController.h"
#include "Enemy/CEnemy.h"
#include "ActorComponents/CStateComponent.h"
#include "ActorComponents/CEnemyAIComponent.h"


UCBTService_Bear::UCBTService_Bear()
{
    this->NodeName = "Bear_Service";  
    this->Interval = 0.1f;     
    this->RandomDeviation = 0.0f;
}
void UCBTService_Bear::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    ACEnemyAIController* controller = Cast<ACEnemyAIController>(OwnerComp.GetOwner());
    ACEnemy* Enemy = Cast<ACEnemy>(controller->GetPawn());

    UCStateComponent* state = Cast<UCStateComponent>(Enemy->GetComponentByClass(UCStateComponent::StaticClass()));
    UCEnemyAIComponent* AIComponent = Cast<UCEnemyAIComponent>(Enemy->GetComponentByClass(UCEnemyAIComponent::StaticClass()));

    if (state->IsHitMode())
    {
        AIComponent->SetHittedMode();
        return;
    }


    ACharacter* target = AIComponent->GetTarget(); // 적 발견 시 
    if (target == nullptr)
    {
        AIComponent->SetRoamMode();
        return;
    }

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