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
	DOREPLIFETIME(ACDestructibleActor, DropItemRatio);
	DOREPLIFETIME(ACDestructibleActor, MaxDamageThreshold);
	DOREPLIFETIME(ACDestructibleActor, DestructibleMesh);

}

void ACDestructibleActor::SetUp(UDestructibleMesh* InDestructibleMesh, FTransform InstanceTransform, float InMaxDamageThreshold, int32 InDropItemRatio)
{
	DestructibleMesh = InDestructibleMesh;
	DestructibleComponent->SetDestructibleMesh(DestructibleMesh);
	DestructibleComponent->SetWorldTransform(InstanceTransform);
	MaxDamageThreshold = InMaxDamageThreshold;
	DropItemRatio = InDropItemRatio;
	FString debugText = TEXT("SetUp Finished");
	CDebug::Print(debugText);

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
	if (HasAuthority()) 
	{
		float prevAccumulatedDamage = AccumulatedDamage;
		AccumulatedDamage += DamageAmount;
		AccumulatedDamage = FMath::Clamp(AccumulatedDamage, 0.0f, MaxDamageThreshold);
		int32 DropItemCount = (int32)((AccumulatedDamage - prevAccumulatedDamage) / DropItemRatio);
		if (DropItemCount > 0)
		{
			//Give Attacker Item by DropItemCount (인벤토리 구현)
		}


		BroadcastAccumulateDamage(DamageAmount);


		if (AccumulatedDamage >= MaxDamageThreshold)
		{
			DestructibleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			DestructibleComponent->SetEnableGravity(true);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACDestructibleActor::DestroyDestructibleActor, 3.0f, false);

		}

	}
	
}

void ACDestructibleActor::BroadcastAccumulateDamage_Implementation(float NewAccumulatedDamage)
{
	this->GetDestructibleComponent()->ApplyRadiusDamage(NewAccumulatedDamage, GetActorLocation(), 1.0f, 1.0f, true);
}



void ACDestructibleActor::OnRep_AccumulateDamage()
{
	FString tempStr = FString::Printf(TEXT(" OnClient Destructible Actor Has AccumulatedDamage %f"), AccumulatedDamage);
	CDebug::Print(tempStr);
}

void ACDestructibleActor::OnRef_DestructibleMeshSet()
{
	if (DestructibleComponent != nullptr)
		DestructibleComponent->SetDestructibleMesh(DestructibleMesh);
}

void ACDestructibleActor::DestroyDestructibleActor()
{
	Destroy();
}



// Called when the game starts or when spawned
void ACDestructibleActor::BeginPlay()
{
	Super::BeginPlay();
	FString tempStr = FString::Printf(TEXT("DestrucibleActor Spawned!!!!!!!!!!!!!"));
	CDebug::Print(tempStr);
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




