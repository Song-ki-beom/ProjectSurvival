// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/CAnimNotify_Move.h"
#include "Character/CSurvivor.h"
#include "ActorComponents/CMovingComponent.h"

void UCAnimNotify_Move::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp == nullptr) return;
	if (MeshComp->GetOwner() == nullptr) return;

	ACSurvivor* survivor = Cast<ACSurvivor>(MeshComp->GetOwner());

	if (survivor)
	{
		survivor->GetMovingComponent()->Move();
	}
}
