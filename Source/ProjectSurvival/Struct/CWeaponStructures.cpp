// Fill out your copyright notice in the Description page of Project Settings.


#include "Struct/CWeaponStructures.h"
#include "GameFramework/Character.h"
//#include "Components/CStateComponent.h"
//#include "Components/CMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/CapsuleComponent.h"

//#include "Components/CMontagesComponent.h"


void FDoActionData::DoAction(ACharacter* InOwner)
{
	/*UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(InOwner);
	if (!!movement)
	{
		if (this->bFixedCamera)
			movement->EnableFixedCamera();
		if (this->bCanMove)
			movement->Stop();
	}*/
	if (this->Montage)
	{
		/*UCMontagesComponent* montagesComponent;
		montagesComponent = CHelpers::GetComponent<UCMontagesComponent>(InOwner);
		if (!!montagesComponent)
			montagesComponent->Montage_Play(this->Montage, this->PlayRate);*/

		//몽타주 컴포넌트 구현 전까지 사용할 플레이 몽타주
			InOwner->PlayAnimMontage(this->Montage, this->PlayRate);
	}
}

void FHitData::SendDamage(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{



	FActionDamageEvent e;
	e.HitData = this;
	// 현재Hit된 구조체를 보냄
	InOther->TakeDamage(this->Power, e, InAttacker->GetController(), InAttackCauser);
}

void FHitData::PlayMontage(ACharacter* InOwner)
{
	/*if(Montage= nullptr ) return;
	if(InOwner = nullptr) return;

	UCMontagesComponent* montagesComponent;
	montagesComponent = CHelpers::GetComponent<UCMontagesComponent>(InOwner);
	if (!!montagesComponent)
		montagesComponent->Montage_Play(this->Montage, this->PlayRate);*/

}

void FHitData::PlayHitStop(UWorld* InWorld)
{
	StopPawns.Empty();
	if(FMath::IsNearlyZero(StopTime)==true) return;

	for (AActor* actor : InWorld->GetCurrentLevel()->Actors)
	{
		APawn* pawn = Cast<ACharacter>(actor);
		if (!!pawn)
		{
			pawn->CustomTimeDilation = 1e-3f; 
			StopPawns.Add(pawn);
		}
	}

	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([=]()
		{
			for (APawn* pawn : StopPawns)
			{
				pawn->CustomTimeDilation = 1;
			}

		});
	FTimerHandle handle;
	InWorld->GetTimerManager().SetTimer(handle, timerDelegate, StopTime, false);
}

void FHitData::PlaySoundWave(ACharacter* InOwner)
{
	if(Sound==nullptr) return;
	if(InOwner== nullptr) return;

	UWorld* world = InOwner->GetWorld();
	FVector location = InOwner->GetActorLocation();

	UGameplayStatics::SpawnSoundAtLocation(world, Sound, location);

}


void FHitData::PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotation, USkeletalMeshComponent* InMesh, FName InSocketName)
{
	if (InMesh == nullptr || Effect == nullptr) return;
	FTransform  transform;
	transform.SetLocation(InLocation + InRotation.RotateVector(EffectLocation));

	//파티클 스폰 
	UParticleSystem* particle = Cast<UParticleSystem>(Effect);
	FVector  location = transform.GetLocation();
	FRotator rotation = FRotator(transform.GetRotation());
	FVector scale = transform.GetScale3D();
	if (!!particle)   
		UGameplayStatics::SpawnEmitterAttached(particle, InMesh, InSocketName, location, rotation, scale);

}
