#pragma warning(push)
#pragma warning(disable : 4996)
#pragma warning(disable : 4706)



#include "Weapon/CDoAction_Axe.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "Utility/CDebug.h"
#include "Environment/CDestructibleActor.h"
#include "Struct/DestructibleStruct.h"
#include "DestructibleComponent.h"



void UCDoAction_Axe::DoAction()
{
	Super::DoAction();
	if (DoActionDatas.Num() > 0)
		DoActionDatas[0].DoAction(OwnerCharacter);
}


void UCDoAction_Axe::Slash()
{
	if (!OwnerCharacter) return;

	//Trace 관련 세팅
	FVector Start = FVector(OwnerCharacter->GetActorLocation().X + 180, OwnerCharacter->GetActorLocation().Y, 180.0f);
	FVector End = Start + OwnerCharacter->GetActorForwardVector() * TraceDistance;
	FQuat Rot = FQuat::Identity;
	FVector HalfSize = FVector(150.0f, 150.0f, 150.0f);
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
		FString hitIndex = *HitResult.Component->GetName().Right(1);
		FString debugText = TEXT("Hitted Polige Mesh Type") + hitIndex;
		CDebug::Print(debugText, FColor::Blue);

		if (CheckIsFoliageInstance(HitResult))
		{

			SwitchFoligeToDestructible(&hitIndex);

		}

	}
}


bool UCDoAction_Axe::CheckIsFoliageInstance(const FHitResult& Hit)
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

void UCDoAction_Axe::SwitchFoligeToDestructible(FString* hitIndex)
{
	if (DestructibleDataTable)
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