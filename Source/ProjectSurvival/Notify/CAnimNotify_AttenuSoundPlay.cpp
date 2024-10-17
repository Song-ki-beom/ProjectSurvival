// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/CAnimNotify_AttenuSoundPlay.h"
#include "Kismet/GameplayStatics.h"

FString  UCAnimNotify_AttenuSoundPlay::GetNotifyName_Implementation() const
{
	return "AttenuSound";
}

void UCAnimNotify_AttenuSoundPlay::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp == nullptr) return;
	if (MeshComp->GetOwner() == nullptr) return;

	UWorld* world = MeshComp->GetOwner()->GetWorld();
	FVector location = MeshComp->GetOwner()->GetActorLocation();
	if (Sound == nullptr) return;

	if (SoundAttenuation)
		UGameplayStatics::SpawnSoundAtLocation(world, Sound, location, FRotator::ZeroRotator, SoundMultiplier, 1.0f, 0.0f, SoundAttenuation);
	else
		UGameplayStatics::SpawnSoundAtLocation(world, Sound, location, FRotator::ZeroRotator,SoundMultiplier);


	
}