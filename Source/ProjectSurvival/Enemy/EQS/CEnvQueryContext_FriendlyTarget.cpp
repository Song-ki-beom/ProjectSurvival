// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EQS/CEnvQueryContext_FriendlyTarget.h"
#include "Enemy/CEnemyAIController.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
void UCEnvQueryContext_FriendlyTarget::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
    AActor* QuerierActor = Cast<AActor>(QueryInstance.Owner.Get());
    if (!QuerierActor) return;

    ACEnemyAIController* AIController = Cast<ACEnemyAIController>(QuerierActor->GetInstigatorController());
    if (!AIController) return;

    AActor* FriendlyTarget = AIController->GetFriendlyTarget();  // GetFriendlyTarget() 함수는 사용자가 정의한 함수입니다.
    if (!FriendlyTarget) return;


    UEnvQueryItemType_Actor::SetContextHelper(ContextData, FriendlyTarget);

}
