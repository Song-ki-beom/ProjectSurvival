// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/CAnimNotifyState_Hit.h"
#include "ActorComponents/CStateComponent.h"
#include "ActorComponents/CMovingComponent.h"
#include "Enemy/CEnemy.h"


FString  UCAnimNotifyState_Hit::GetNotifyName_Implementation() const
{
	return "HitState";
}

void UCAnimNotifyState_Hit::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (MeshComp == nullptr) return;
	if (MeshComp->GetOwner() == nullptr) return;

	Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (Character == nullptr) return;
	UCMovingComponent* MovingComponent = Cast<UCMovingComponent>(MeshComp->GetOwner()->GetComponentByClass(UCMovingComponent::StaticClass()));
	if (MovingComponent == nullptr) return;
	MovingComponent->DisableControlRotation();
	MovingComponent->Stop();
	UCStateComponent* StateComponent = Cast<UCStateComponent>(MeshComp->GetOwner()->GetComponentByClass(UCStateComponent::StaticClass()));
	if (StateComponent == nullptr) return;
	StateComponent->ChangeType(EStateType::Hit);


}
void UCAnimNotifyState_Hit::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (MeshComp == nullptr) return;
	if (MeshComp->GetOwner() == nullptr) return;

	Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (Character == nullptr) return;
	UCMovingComponent* MovingComponent = Cast<UCMovingComponent>(MeshComp->GetOwner()->GetComponentByClass(UCMovingComponent::StaticClass()));
	if (MovingComponent == nullptr) return;
	MovingComponent->EnableControlRotation();
	MovingComponent->Move();
	UCStateComponent* StateComponent = Cast<UCStateComponent>(MeshComp->GetOwner()->GetComponentByClass(UCStateComponent::StaticClass()));
	if (StateComponent == nullptr) return;
	StateComponent->ChangeType(EStateType::Idle);


}