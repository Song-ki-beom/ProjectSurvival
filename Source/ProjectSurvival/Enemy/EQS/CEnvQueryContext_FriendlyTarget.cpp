// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EQS/CEnvQueryContext_FriendlyTarget.h"
#include "Enemy/CEnemyAIController.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
void UCEnvQueryContext_FriendlyTarget::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
    // 쿼리 요청자(Querier)로부터 Actor 가져오기
    AActor* QuerierActor = Cast<AActor>(QueryInstance.Owner.Get());
    if (!QuerierActor) return;

    // Querier Actor로부터 AIController 가져오기
    ACEnemyAIController* AIController = Cast<ACEnemyAIController>(QuerierActor->GetInstigatorController());
    if (!AIController) return;

    // AIController에서 설정된 friendly target 가져오기
    AActor* FriendlyTarget = AIController->GetFriendlyTarget();  // GetFriendlyTarget() 함수는 사용자가 정의한 함수입니다.
    if (!FriendlyTarget) return;


    // 컨트롤된 폰의 타겟 위치를 기준으로 컨텍스트 제공
    UEnvQueryItemType_Actor::SetContextHelper(ContextData, FriendlyTarget);

}
