// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/CAnimNotifyState_EnemyAction.h"
#include "ActorComponents/CStateComponent.h"
#include "ActorComponents/CMovingComponent.h"
#include "Utility/CDebug.h"
#include "Enemy/CEnemy.h"


FString  UCAnimNotifyState_EnemyAction::GetNotifyName_Implementation() const
{
	return "EnemyActionState";
}

void UCAnimNotifyState_EnemyAction::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (MeshComp == nullptr) return;
	if (MeshComp->GetOwner() == nullptr) return;

	Enemy = Cast<ACEnemy>(MeshComp->GetOwner());
	if (Enemy == nullptr) return;
	Enemy->Begin_DoAction();

}
void UCAnimNotifyState_EnemyAction::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (MeshComp == nullptr) return;
	if (MeshComp->GetOwner() == nullptr) return;

	Enemy = Cast<ACEnemy>(MeshComp->GetOwner());
	if (Enemy == nullptr) return;
	Enemy->End_DoAction();
	//CDebug::Print(TEXT("!!!!!!!!!!!!!!Attack Notify Ended!!!!!!!!!!!!"));

}