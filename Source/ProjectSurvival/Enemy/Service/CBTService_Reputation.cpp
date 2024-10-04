// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Service/CBTService_Reputation.h"
#include "GameFramework/Character.h"
#include "Enemy/CEnemyAIController.h"
#include "Enemy/CEnemy.h"
#include "Utility/CDebug.h"
#include "ActorComponents/CStateComponent.h"
#include "ActorComponents/CEnemyAIComponent.h"
#include "ActorComponents/CStatusComponent.h"


UCBTService_Reputation::UCBTService_Reputation()
{
    this->NodeName = "Reputation_Service";
    this->Interval = 0.1f;
    this->RandomDeviation = 0.0f;
}

void UCBTService_Reputation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    ACEnemyAIController* controller = Cast<ACEnemyAIController>(OwnerComp.GetOwner());
    ACEnemy* Enemy = Cast<ACEnemy>(controller->GetPawn());

    UCEnemyAIComponent* AIComponent = Cast<UCEnemyAIComponent>(Enemy->GetComponentByClass(UCEnemyAIComponent::StaticClass()));

    if (AIComponent->IsFriendlyMode())
    {
        AIComponent->SetFriendlyMode();
        return;
    }

    if (AIComponent->IsHostileMode())
    {
        AIComponent->SetHostileMode();
        return;
    }

    
    //CDebug::Print(TEXT("Distance Between Enemy and Player: "), distance);
   


}