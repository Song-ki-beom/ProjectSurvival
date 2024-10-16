// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Decorator/CBTD_IsSurvivorAround.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/CEnemyAIController.h"
#include "ActorComponents/CEnemyAIComponent.h"
#include "Enemy/CEnemy.h"
#include "Character/CSurvivor.h"
#include "World/CPickUp.h"
#include "Widget/Inventory/CItemBase.h"
#include "Interface/InteractionInterface.h"
#include "DrawDebugHelpers.h"


UCBTD_IsSurvivorAround::UCBTD_IsSurvivorAround()
{
	NodeName = TEXT("IsSurvivorAround");
}

bool UCBTD_IsSurvivorAround::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{

	ACEnemy* Enemy = Cast<ACEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (Enemy == nullptr) return false;

	UCEnemyAIComponent* AIComponent = Cast<UCEnemyAIComponent>(Enemy->GetComponentByClass(UCEnemyAIComponent::StaticClass()));
	if (AIComponent == nullptr) return false;

	bool bHasEncountered = OwnerComp.GetBlackboardComponent()->GetValueAsBool(EncounterKey.SelectedKeyName);
	float SearchSphereRadius = 0.0f;
	if (bHasEncountered)
	{ 
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(EncounterKey.SelectedKeyName, false);
		SearchSphereRadius = 1000.0f;
	}
	else
	{
		SearchSphereRadius = 300.0f;
	}

	FVector TraceStart = Enemy->GetActorLocation();

	TArray<FHitResult> HitResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Enemy);

	FCollisionObjectQueryParams ObjectQueryParams; //다중 검출 PARAM
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	// 멀티 스피어 트레이스 실행
	bool bHit = GetWorld()->SweepMultiByObjectType(
		HitResults,
		TraceStart,
		TraceStart,
		FQuat::Identity,
		ObjectQueryParams,
		FCollisionShape::MakeSphere(SearchSphereRadius),
		QueryParams
	);
	DrawDebugSphere(GetWorld(), TraceStart, SearchSphereRadius, 50, FColor::Red, false, 0.1f);


	if (bHit)
	{
		AActor* NewTargetActor = NULL;
		float shortestDistance = 99999.0f;
		for (const FHitResult& Hit : HitResults)
		{

			if (IsValid(Hit.GetActor()))
			{
					if (ACSurvivor* survivor = Cast<ACSurvivor>(Hit.GetActor()))
					{
						float distanceTo = Enemy->GetDistanceTo(Hit.GetActor());
						if (distanceTo < shortestDistance)
						{
							shortestDistance = distanceTo;
							NewTargetActor = Hit.GetActor();
						}
					}
			}
		}

		// 디버그 스피어 그리기
		if (NewTargetActor)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(TargetActorKey.SelectedKeyName, NewTargetActor);
			return true;
		}
		else return false;
	}
	return false;

}