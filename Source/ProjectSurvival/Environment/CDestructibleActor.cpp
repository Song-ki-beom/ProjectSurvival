// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/CDestructibleActor.h"
#include "DestructibleComponent.h"
#include "Struct/CDestructibleStructures.h"
#include "TimerManager.h"
#include "Utility/CDebug.h"
#include "Net/UnrealNetwork.h"
#include "Math/UnrealMathUtility.h"
// Sets default values
ACDestructibleActor::ACDestructibleActor()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;
	DestructibleComponent = CreateDefaultSubobject<UDestructibleComponent>(TEXT("DestructibleMesh")); 
	DestructibleComponent->SetupAttachment(GetRootComponent());
}

void ACDestructibleActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACDestructibleActor, DestructibleComponent);
	DOREPLIFETIME(ACDestructibleActor, AccumulatedDamage);
}

void ACDestructibleActor::SetUp(UDestructibleMesh* InDestructibleMesh, FTransform InstanceTransform, float InMaxDamageThreshold, int32 InDropItemRatio)
{
	DestructibleComponent->SetDestructibleMesh(InDestructibleMesh);
	DestructibleComponent->SetWorldTransform(InstanceTransform);
	MaxDamageThreshold = InMaxDamageThreshold;
	DropItemRatio = InDropItemRatio;
}

class UDestructibleComponent* ACDestructibleActor::GetDestructibleComponent()
{
	return DestructibleComponent;
}



float ACDestructibleActor::GetAccumulatedDamage()
{
	return AccumulatedDamage;
}

void ACDestructibleActor::AccumulateDamage(float DamageAmount)
{
	if (HasAuthority()) {
		float prevAccumulatedDamage = AccumulatedDamage;
		AccumulatedDamage += DamageAmount;
		AccumulatedDamage = FMath::Clamp(AccumulatedDamage, 0.0f, MaxDamageThreshold);
		FString tempStr = FString::Printf(TEXT(" OnServer this Actor Has AccumulatedDamage %f"), AccumulatedDamage);
		CDebug::Print(tempStr);
		int32 DropItemCount = (int32)((AccumulatedDamage - prevAccumulatedDamage) / DropItemRatio);
		OnRep_AccumulateDamage(AccumulatedDamage);
		if (DropItemCount > 0)
		{
			//Give Attacker Item by DropItemCount (인벤토리 구현하면)
		}



		if (AccumulatedDamage >= MaxDamageThreshold)
		{
			DestructibleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			DestructibleComponent->SetEnableGravity(true);
			DestructibleComponent->GetDestructibleMesh()->
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACDestructibleActor::DestroyDestructibleActor, 3.0f, false);

		}

	}
	else
	{
		OnServer_AccumulateDamage(AccumulatedDamage);
	}
}

void ACDestructibleActor::OnServer_AccumulateDamage(float NewAccumulatedDamage)
{
	AccumulateDamage(NewAccumulatedDamage);
}

void ACDestructibleActor::OnRep_AccumulateDamage(float NewAccumulatedDamage)
{
	FString tempStr = FString::Printf(TEXT(" OnClient Actor Has AccumulatedDamage %f"), NewAccumulatedDamage);
	CDebug::Print(tempStr);
}

void ACDestructibleActor::DestroyDestructibleActor()
{
	Destroy();
}



// Called when the game starts or when spawned
void ACDestructibleActor::BeginPlay()
{
	Super::BeginPlay();
	DestructibleComponent->RegisterComponent();
	DestructibleComponent->AddToRoot(); // Root Set 등록 ->가비지 컬렉션 삭제 방지
}

void ACDestructibleActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (DestructibleComponent)
	{
		DestructibleComponent->RemoveFromRoot(); // Root Set 등록 해제
		DestructibleComponent->DestroyComponent();

	}
	Super::EndPlay(EndPlayReason);

}




