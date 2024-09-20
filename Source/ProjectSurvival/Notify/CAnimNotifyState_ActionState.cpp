// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/CAnimNotifyState_ActionState.h"
#include "ActorComponents/CStateComponent.h"



FString  UCAnimNotifyState_ActionState::GetNotifyName_Implementation() const
{
	return "Begin_DoAction";
}

void UCAnimNotifyState_ActionState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (MeshComp == nullptr) return;
	if (MeshComp->GetOwner() == nullptr) return;

	UCStateComponent* StateComponent = Cast<UCStateComponent>(MeshComp->GetOwner()->GetComponentByClass(UCStateComponent::StaticClass()));

	if (StateComponent == nullptr) return;
	StateComponent->ChangeType(EStateType::Action);

}
void UCAnimNotifyState_ActionState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);


	if (MeshComp == nullptr) return;
	if (MeshComp->GetOwner() == nullptr) return;

	UCStateComponent* StateComponent = Cast<UCStateComponent>(MeshComp->GetOwner()->GetComponentByClass(UCStateComponent::StaticClass()));

	if (StateComponent == nullptr) return;
	StateComponent->ChangeType(EStateType::Idle);

}