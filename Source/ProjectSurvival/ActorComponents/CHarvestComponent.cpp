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
#include "Struct/DestructibleStruct.h"
#include "Environment/CDestructibleActor.h"
#include "DestructibleComponent.h"
#include "CGameInstance.h"
#include "Kismet/GameplayStatics.h"
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


// Called every frame
void UCHarvestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}



void UCHarvestComponent::HarvestBoxTrace()
{
	if (!OwnerCharacter) return;

	//Trace 관련 세팅
	FVector ForwardVector = OwnerCharacter->GetActorForwardVector();
	FVector Start = FVector(OwnerCharacter->GetActorLocation().X, OwnerCharacter->GetActorLocation().Y, OwnerCharacter->GetActorLocation().Z+45.0f)+ ForwardVector.GetSafeNormal() * TraceOffset;
	FVector End = Start + ForwardVector.GetSafeNormal()* TraceDistance;
	FQuat Rot = FQuat(OwnerCharacter->GetActorRotation());

	FVector HalfSize = FVector(TraceDistance/2.0f, TraceDistance/ 2.0f, TraceDistance/ 2.0f);
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



	if (bHit)
	{
		FString hitIndex = *HitResult.Component->GetName().Right(2);
		//FString hitIndex = *HitResult.Component->GetName();

		FString debugText = TEXT("Hitted Polige Mesh Type ") + hitIndex;
		CDebug::Print(debugText, FColor::Blue);

		if (CheckIsFoliageInstance(HitResult))
		{

			SwitchFoligeToDestructible(&hitIndex);

		}
		else if (CheckIsDestructInstance(HitResult))
		{
			AddForceToDestructible(20.0f);
		}

	}
}


bool UCHarvestComponent::CheckIsFoliageInstance(const FHitResult& Hit)
{

	if (UInstancedStaticMeshComponent* InstancedMesh = Cast<UInstancedStaticMeshComponent>(Hit.Component))
	{
		InstanceIndex = Hit.Item;
		FString debugText = TEXT("Hitted Polige Mesh Index") + FString::FromInt(InstanceIndex);
		CDebug::Print(debugText);
		InstancedMesh->GetInstanceTransform(InstanceIndex, SpawnTransform, true);
		InstancedMesh->RemoveInstance(InstanceIndex);
		if (InstanceIndex != NO_INDEX) return true;

	}
	else
	{
		CDebug::Print(TEXT("Cannot Convert to FOlige Mesh"));
	}
	return false;

}

bool UCHarvestComponent::CheckIsDestructInstance(const FHitResult& Hit)
{
	if (DestructibleActor = Cast<ACDestructibleActor>(Hit.Actor))
	{
		return true;
	}


	return false;
}

void UCHarvestComponent::SwitchFoligeToDestructible(FString* hitIndex)
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
			FVector SpawnLocation = SpawnTransform.GetLocation();
			FRotator SpawnRotation = FRotator(SpawnTransform.GetRotation());
			FActorSpawnParameters SpawnParams;

			// Spawn ADestructibleActor
			ACDestructibleActor* destructibleActor = GetWorld()->SpawnActor<ACDestructibleActor>(ACDestructibleActor::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
			destructibleActor->SetDestructibleMesh(Row->DestructibleMesh, SpawnTransform);
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

void UCHarvestComponent::AddForceToDestructible(float damageAmount)
{
	
	if(DestructibleActor)
		DestructibleActor->GetDestructibleComponent()->ApplyDamage(damageAmount, DestructibleActor->GetActorLocation(), FVector::ZeroVector, 0.02f);



	/*AddRadialForce(FVector Origin,
		float Radius,
		float Strength,
		ERadialImpulseFalloff Falloff,
		bool bAccelChange
	)*/

		if (DestructibleActor->GetDestructibleComponent()->GetDamage >= DestructibleComponent->GetFractureThreshold())
		{
			UE_LOG(LogTemp, Log, TEXT("Fracture occurred!"));
			// 추가적인 로직을 이곳에 작성
		}
	


}
