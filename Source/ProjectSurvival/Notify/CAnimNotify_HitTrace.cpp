// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/CAnimNotify_HitTrace.h"
#include "Enemy/CEnemy.h"

FString  UCAnimNotify_HitTrace::GetNotifyName_Implementation() const
{
	return "AttackHitTrace";

}

void UCAnimNotify_HitTrace::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);


	if (MeshComp == nullptr) return;
	if (MeshComp->GetOwner() == nullptr) return;
	ACEnemy* Enemy = Cast<ACEnemy>(MeshComp->GetOwner());


	if (Enemy == nullptr) return;
	if(Enemy->HasAuthority())
		Enemy->AttackTraceHit();
	

}