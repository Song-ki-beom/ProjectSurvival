// Fill out your copyright notice in the Description page of Project Settings.
#pragma warning(push)
#pragma warning(disable : 4996)
#pragma warning(disable : 4706)


#include "ActorComponents/CHarvestComponent.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "Utility/CDebug.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Struct/CDestructibleStructures.h"
#include "Environment/CDestructibleActor.h"
#include "DestructibleComponent.h"
#include "CGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


UCHarvestComponent::UCHarvestComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	GameInstance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(this));
	

}


void UCHarvestComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	
}


void UCHarvestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


void UCHarvestComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCHarvestComponent, HitIndex);
	DOREPLIFETIME(UCHarvestComponent, DamageAmount);
	DOREPLIFETIME(UCHarvestComponent, SpawnTransform);
	DOREPLIFETIME(UCHarvestComponent, InstanceIndex);
	DOREPLIFETIME(UCHarvestComponent, InstanceToRemove);


}


void UCHarvestComponent::HarvestBoxTrace(float InDamageAmount)
{
	if (!OwnerCharacter) return;

	
	
		//Trace 관련 세팅
		FVector ForwardVector = OwnerCharacter->GetActorForwardVector();
		FVector Start = FVector(OwnerCharacter->GetActorLocation().X, OwnerCharacter->GetActorLocation().Y, OwnerCharacter->GetActorLocation().Z + 45.0f) + ForwardVector.GetSafeNormal() * TraceOffset;
		FVector End = Start + ForwardVector.GetSafeNormal() * TraceDistance;
		FQuat Rot = FQuat(OwnerCharacter->GetActorRotation());

		FVector HalfSize = FVector(TraceDistance / 2.0f, TraceDistance / 2.0f, TraceDistance / 2.0f);
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(OwnerCharacter);

		//BoxTrace 
		bool bHit = GetWorld()->SweepSingleByChannel(
			HitResult,
			Start,
			End,
			Rot,
			ECC_WorldStatic,
			FCollisionShape::MakeBox(HalfSize),
			CollisionParams
		);

		DrawDebugBox(GetWorld(), Start, HalfSize, Rot, FColor::Red, false, 1.0f);
		DrawDebugBox(GetWorld(), End, HalfSize, Rot, FColor::Red, false, 1.0f);

		//커밋 테스트(09.03)

		if (bHit)
		{
			FString hitIndex = *HitResult.Component->GetName().Right(2);
			//FString hitIndex = *HitResult.Component->GetName();

			FString debugText = TEXT("Hitted Polige Mesh Type ") + hitIndex;
			CDebug::Print(debugText, FColor::Blue);

			if (CheckIsFoliageInstance(HitResult))
			{
				if (OwnerCharacter->HasAuthority()) 
				{
					SwitchFoligeToDestructible(hitIndex, InDamageAmount,SpawnTransform);
				}
				else
				{
					RequestSwitchFoligeToDestructible(hitIndex,InDamageAmount,SpawnTransform);
				}

			}
			else if (CheckIsDestructInstance(HitResult))
			{

					AddForceToDestructible(InDamageAmount , DestructibleActor);
			}

		}

	
	
}


bool UCHarvestComponent::CheckIsFoliageInstance(const FHitResult& Hit)
{

	if (InstanceToRemove = Cast<UInstancedStaticMeshComponent>(Hit.Component))
	{
		InstanceIndex = Hit.Item;
		FString debugText = TEXT("Hitted Polige Mesh Index") + FString::FromInt(InstanceIndex);
		CDebug::Print(debugText);
		InstanceToRemove->GetInstanceTransform(InstanceIndex, SpawnTransform, true);
		if (OwnerCharacter->HasAuthority())
		{
			RemoveFoliageInstance(InstanceToRemove,InstanceIndex);
		}
		else
		{
			RequestRemoveFoliageInstance(InstanceToRemove, InstanceIndex);
		}
			

		if (InstanceIndex != NO_INDEX) return true;

	}
	else
	{
		CDebug::Print(TEXT("Cannot Convert to FOlige Mesh"));
	}
	return false;

}

void UCHarvestComponent::RemoveFoliageInstance(UInstancedStaticMeshComponent* InInstanceToRemove, int32 InInstanceIndex)
{
	
	InInstanceToRemove->RemoveInstance(InInstanceIndex);
	
	if (OwnerCharacter->HasAuthority()) 
	{
		BroadCastRemoveFoliageInstance(InInstanceToRemove, InInstanceIndex);
	}
	
	

}

void UCHarvestComponent::RequestRemoveFoliageInstance_Implementation(UInstancedStaticMeshComponent* InInstanceToRemove, int32 InInstanceIndex)
{
	
	RemoveFoliageInstance(InInstanceToRemove, InInstanceIndex);
}

void UCHarvestComponent::BroadCastRemoveFoliageInstance_Implementation(UInstancedStaticMeshComponent* InInstanceToRemove, int32 InInstanceIndex)
{
	InInstanceToRemove->RemoveInstance(InInstanceIndex);

}


bool UCHarvestComponent::CheckIsDestructInstance(const FHitResult& Hit)
{
	if (DestructibleActor = Cast<ACDestructibleActor>(Hit.Actor))
	{
		return true;
	}
	else
	{
		CDebug::Print(TEXT("Cannot cast To DestructibleActor"));

	}

	return false;
}

void UCHarvestComponent::SwitchFoligeToDestructible(const FString& hitIndex, float damageAmount, FTransform InSpawnTransform)
{
	

		UDataTable* DestructibleDataTable = nullptr;
		if (GameInstance)
		{
			DestructibleDataTable = GameInstance->DestructibleDataTable;
		}

		if (DestructibleDataTable != nullptr)
		{
			FDestructibleStruct* Row = DestructibleDataTable->FindRow<FDestructibleStruct>(FName(*hitIndex), FString(""));
			if (Row && Row->DestructibleMesh)
			{
				FVector SpawnLocation = InSpawnTransform.GetLocation();
				FRotator SpawnRotation = FRotator(InSpawnTransform.GetRotation());
				FActorSpawnParameters SpawnParams;

				// Spawn ADestructibleActor
				ACDestructibleActor* destructibleActor = GetWorld()->SpawnActor<ACDestructibleActor>(ACDestructibleActor::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
				destructibleActor->SetUp(Row->DestructibleMesh, InSpawnTransform, Row->MaxDamageThreshold, Row->DropItemRatio);
				destructibleActor->AccumulateDamage(damageAmount);
				
		

			}
			else
			{
				CDebug::Print(TEXT("Spawn Failed"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Data Table is null"));
		}

		
	
	
}


void UCHarvestComponent::RequestSwitchFoligeToDestructible_Implementation(const FString& InHitIndex, float IndamageAmount, FTransform InSpawnTransform)
{
	FString tempStr = FString::Printf(TEXT(" OnServer this Actor should apply  %f"), IndamageAmount);
	CDebug::Print(tempStr);
	SwitchFoligeToDestructible(InHitIndex, IndamageAmount, InSpawnTransform);

}



void UCHarvestComponent::BroadcastSwitchFoligeToDestructible_Implementation(UDestructibleMesh* InDestructibleMesh, FTransform InstanceTransform, float InMaxDamageThreshold, int32 InDropItemRatio, float InDamageAmount)
{
	//SwitchFoligeToDestructible(InHitIndex, IndamageAmount, InSpawnTransform);
}





void UCHarvestComponent::AddForceToDestructible(float IndamageAmount , class ACDestructibleActor* InDestructibleActor)
{
	if (OwnerCharacter->HasAuthority()&& InDestructibleActor)
	{
		InDestructibleActor->AccumulateDamage(IndamageAmount);
	}
	else
	{
		RequestAddForceToDestructible(IndamageAmount , InDestructibleActor);
	}
	DestructibleActor = nullptr;

}

void UCHarvestComponent::RequestAddForceToDestructible_Implementation(float IndamageAmount, class ACDestructibleActor* InDestructibleActor)
{
	AddForceToDestructible(IndamageAmount , InDestructibleActor);
}

