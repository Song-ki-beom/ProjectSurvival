// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/CDestructibleActor.h"
#include "DestructibleComponent.h"
#include "TimerManager.h"
#include "Utility/CDebug.h"
#include "World/CPickUp.h"
#include "Widget/Inventory/CItemBase.h"
#include "Net/UnrealNetwork.h"
#include "Math/UnrealMathUtility.h"
// Sets default values
ACDestructibleActor::ACDestructibleActor()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;
	DestructibleComponent = CreateDefaultSubobject<UDestructibleComponent>(TEXT("DestructibleMesh")); 
	RootComponent = DestructibleComponent;
	DropItemID = FName("Harvest_1");
	MaxDamageThreshold = 100.0f;
	DropItemNum = 3.0f;
	DropOffsetRange = 50.0f;
	

	


	ConstructorHelpers::FObjectFinder<UDataTable> DataTableAsset(TEXT("DataTable'/Game/PirateIsland/Include/Datas/Widget/Inventory/DT_Items.DT_Items'"));
	if (DataTableAsset.Succeeded())
	{
		ItemDataTable = DataTableAsset.Object;
	}
}

void ACDestructibleActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACDestructibleActor, DestructibleComponent);
	DOREPLIFETIME(ACDestructibleActor, AccumulatedDamage);
	DOREPLIFETIME(ACDestructibleActor, EarnItemRatio);
	DOREPLIFETIME(ACDestructibleActor, MaxDamageThreshold);
	DOREPLIFETIME(ACDestructibleActor, DestructibleMesh);

}

void ACDestructibleActor::SetUp(FTransform InstanceTransform, FDestructibleStruct* DestructibleStruct)
{
	DestructibleMesh = DestructibleStruct->DestructibleMesh;

	DestructibleComponent->SetDestructibleMesh(DestructibleMesh);
	DestructibleComponent->SetWorldTransform(InstanceTransform);
	MaxDamageThreshold = DestructibleStruct->MaxDamageThreshold;
	EarnItemRatio = DestructibleStruct->EarnItemRatio;
	DropItemID = DestructibleStruct->DropItemID;
	DropItemNum = DestructibleStruct->DropItemNum;
	DropOffsetRange = DestructibleStruct->DropOffsetRange;
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

		BroadcastAccumulateDamage(DamageAmount);


		if (AccumulatedDamage >= MaxDamageThreshold)
		{
			//DestructibleMesh 콜리전 off 처리 
			DestructibleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			DestructibleComponent->SetEnableGravity(true);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACDestructibleActor::DestroyDestructibleActor, 1.7f, false);

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
	//CDebug::Print(tempStr);
}

void ACDestructibleActor::OnRef_DestructibleMeshSet()
{
	if (DestructibleComponent != nullptr)
		DestructibleComponent->SetDestructibleMesh(DestructibleMesh);
}

void ACDestructibleActor::DestroyDestructibleActor()
{
	//드랍 아이템 구현 
	CreateDropItem();
	Destroy();
}

void ACDestructibleActor::CreateDropItem()
{



	const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DropItemID, DropItemID.ToString());
	if (ItemData)
	{
		UCItemBase* ItemToDrop = NewObject<UCItemBase>(UCItemBase::StaticClass());
		ItemToDrop->CopyFromItemData(*ItemData);
	}

		
		// 디스트럭티블 메쉬의  절반 높이 가져오기 (Z 축 기준)
		FVector MeshBounds = DestructibleComponent->Bounds.BoxExtent;
		float HalfHeight = MeshBounds.Z;
		float DropHeightRatio = 0.5f;
		// 스폰 위치 설정 시 절반 높이를 고려하여 Y축에만 오프셋을 적용
		if (ItemData->ID == "Harvest_1")
		{
			DropHeightRatio = 0.7f;
		}
		else if (ItemData->ID == "Harvest_2")
		{
			DropHeightRatio = 0.3f;
		}

		FVector SpawnLocation = this->GetActorLocation() + FVector(0.0f, 0.0f, HalfHeight*DropHeightRatio);

	for (int i = 0; i < DropItemNum; i++)
	{
		// 겹치지 않도록 랜덤 오프셋 추가
		FVector RandomOffset = FVector(
			FMath::RandRange(-DropOffsetRange, DropOffsetRange),
			FMath::RandRange(-DropOffsetRange, DropOffsetRange),
			0.0f //높이는 그대로 
		);
		FVector FinalSpawnLocation = SpawnLocation + RandomOffset;
		FTransform SpawnTransform(this->GetActorRotation(), FinalSpawnLocation); //최종 스폰 위치 
		
		//스폰 파라미터 설정 
		FActorSpawnParameters SpawnParams;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn; // 벽에 끼이는 등  spawn 에 실패하는 상황이 생기면 위치를 Adjust해서 성공시킴 

		ACPickUp* Pickup = GetWorld()->SpawnActor<ACPickUp>(ACPickUp::StaticClass(), SpawnTransform, SpawnParams);
		Pickup->InitializeDrop(DropItemID, 1);

	}
	

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




