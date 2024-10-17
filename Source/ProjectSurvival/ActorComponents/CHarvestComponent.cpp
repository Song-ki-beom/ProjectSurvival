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
#include "Particles/ParticleSystem.h"

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


void UCHarvestComponent::ApplyHarvestEvent(FHitResult HitResult,float InDamageAmount, EWeaponType CauserWeaponType)
{
		

			FString hitObjectName = *HitResult.Component->GetName();


			int32 StringLength = hitObjectName.Len();

			// 뒤에서부터 숫자를 추출
			int32 StartIndex = StringLength - 1;
			while (StartIndex >= 0 && FChar::IsDigit(hitObjectName[StartIndex]))
			{
				StartIndex--;
			}


			// 숫자 부분만 추출 (시작 인덱스를 증가시켜야 숫자 부분을 정확히 가져올 수 있음)
			FString hitIndex = hitObjectName.Right(StringLength - StartIndex - 1);

			FString debugText = TEXT("Hitted Polige Mesh Type ") + hitIndex;
			CDebug::Print(debugText, FColor::Blue);

			float FinalDamageAmount = InDamageAmount;
			if ((CauserWeaponType == EWeaponType::IronPick || CauserWeaponType == EWeaponType::StonePick) && (hitIndex == "0"))
			{
				FinalDamageAmount *= 1.5f;
			}
			else if (CauserWeaponType == EWeaponType::StoneAxe || CauserWeaponType == EWeaponType::IronAxe)
			{
				if (hitIndex == "8" || hitIndex == "7" || hitIndex == "9")
				{
					FinalDamageAmount *= 1.4f;
				}
			}
			else
			{
				FinalDamageAmount *= 1.0f;
			}
			


			//PlayChopEffect
			if (OwnerCharacter->HasAuthority())
				BroadcastPlayHarvestEffect(HitResult);


			if (CheckIsFoliageInstance(HitResult))
			{
				
					SwitchFoligeToDestructible(hitIndex, FinalDamageAmount,SpawnTransform);
				

			}
			else if (CheckIsDestructInstance(HitResult))
			{

					AddForceToDestructible(FinalDamageAmount, DestructibleActor);
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
	
	if (OwnerCharacter->HasAuthority()) 
	{
		BroadCastRemoveFoliageInstance(InInstanceToRemove, InInstanceIndex);
	}
	

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
				if (destructibleActor)
				{
					//CDebug::Print(TEXT("Destructible Actor Spawn Succeeded"));

				}
				else
				{
					CDebug::Print(TEXT("Destructible Actor Spawn Failed"));

				}

				destructibleActor->SetUp(InSpawnTransform,Row);
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

void UCHarvestComponent::BroadcastPlayHarvestEffect_Implementation(const FHitResult HitResult)
{
	if (HarvestParticle == nullptr) return;
	FVector hitLocation = HitResult.ImpactPoint; // 히트된 지점의 위치
	UGameplayStatics::SpawnEmitterAtLocation(
		OwnerCharacter->GetWorld(),
		HarvestParticle,  
		hitLocation,     
		FRotator::ZeroRotator,
		true           
	);
}

void UCHarvestComponent::RequestAddForceToDestructible_Implementation(float IndamageAmount, class ACDestructibleActor* InDestructibleActor)
{
	AddForceToDestructible(IndamageAmount , InDestructibleActor);
}

