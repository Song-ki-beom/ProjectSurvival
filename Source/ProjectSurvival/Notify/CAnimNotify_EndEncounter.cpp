// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/CAnimNotify_EndEncounter.h"
#include "Enemy/CEnemyAIController.h"
#include "Enemy/CEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ActorComponents/CEnemyAIComponent.h"
#include "ActorComponents/CMovingComponent.h"
#include "ActorComponents/CStateComponent.h"
#include "Components/BoxComponent.h"
#include "Utility/CDebug.h"
#include "Enemy/CEnemyAIController.h"
FString  UCAnimNotify_EndEncounter::GetNotifyName_Implementation() const
{
	return "EndEncounter";
}

void UCAnimNotify_EndEncounter::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp == nullptr) return;
	if (MeshComp->GetOwner() == nullptr) return;
	UCEnemyAIComponent* EnemyAIComponent = Cast<UCEnemyAIComponent>(MeshComp->GetOwner()->GetComponentByClass(UCEnemyAIComponent::StaticClass()));
	if(MeshComp->GetOwner()->HasAuthority())
		EnemyAIComponent->GetBlackboard()->SetValueAsBool(FName("bHasEncountered"), true);

}