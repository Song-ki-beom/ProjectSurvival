// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Decorator/CBTD_IsFoodAround.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/CEnemyAIController.h"
#include "ActorComponents/CEnemyAIComponent.h"
#include "Enemy/CEnemy.h"
#include "World/CPickUp.h"
#include "Widget/Inventory/CItemBase.h"
#include "Interface/InteractionInterface.h"
#include "DrawDebugHelpers.h"


UCBTD_IsFoodAround::UCBTD_IsFoodAround()
{
	NodeName = TEXT("IsFoodAround");
}

bool UCBTD_IsFoodAround::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{



	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControlledPawn) return false;

	UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetFoodActorKey.SelectedKeyName);
	if (TargetObject) return  true;



	float SphereRadius = 700.0f; // 탐색 반지름 설정

	FVector TraceStart = ControlledPawn->GetActorLocation();

	TArray<FHitResult> HitResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(ControlledPawn);

	// 멀티 스피어 트레이스 실행
	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		TraceStart,
		TraceStart,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(SphereRadius),
		QueryParams
	);
	//DrawDebugSphere(GetWorld(), TraceStart, SphereRadius, 50, FColor::Green, false, 0.1f);

	// 트레이스된 PickUp Actor들을 담을 TArray
	TArray<ACPickUp*> PickUpsInSphere;

	if (bHit)
	{
		AActor* NewTargetActor = NULL;
		float shortestDistance = 99999.0f;
		for (const FHitResult& Hit : HitResults)
		{

			if (IsValid(Hit.GetActor()) && Hit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
			{

				ACPickUp* pickUpActor = Cast<ACPickUp>(Hit.GetActor());
				if (pickUpActor)
				{
					if (pickUpActor->ItemReference->ItemType == EItemType::Consumable && (pickUpActor->ItemReference->ID == FName("Consumable_1") || pickUpActor->ItemReference->ID == FName("Consumable_2")))
					{
						float distanceTo = ControlledPawn->GetDistanceTo(pickUpActor);
						if (distanceTo < shortestDistance)
						{
							shortestDistance = distanceTo;
							NewTargetActor = pickUpActor;
						}
					}
				}
			}
		}
		// 디버그 스피어 그리기
		if (NewTargetActor)
		{

			ACEnemyAIController* controller = Cast<ACEnemyAIController>(OwnerComp.GetOwner());
			if (controller == nullptr) return false;
			ACEnemy* Enemy = Cast<ACEnemy>(controller->GetPawn());
			if (Enemy == nullptr) return false;
			UCEnemyAIComponent* AIComponent = Cast<UCEnemyAIComponent>(Enemy->GetComponentByClass(UCEnemyAIComponent::StaticClass()));
			if (AIComponent == nullptr) return false;
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(TargetFoodActorKey.SelectedKeyName, NewTargetActor);
			return true;
		}
		else return false;
	}
	return false;

}