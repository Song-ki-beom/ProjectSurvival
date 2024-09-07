// Fill out your copyright notice in the Description page of Project Settings.


#include "World/CPickUp.h"
#include "Character/CSurvivor.h"
#include "ActorComponents/CInventoryComponent.h"
#include "Utility/CDebug.h"
#include "Engine/DataTable.h"
#include "ActorComponents/CInteractionComponent.h"
#include "Net/UnrealNetwork.h"
#include "Widget/Inventory/CItemBase.h"

// Sets default values
ACPickUp::ACPickUp()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	PickupMesh->SetSimulatePhysics(false);
	PickupMesh->SetEnableGravity(true);
	SetRootComponent(PickupMesh);
	ConstructorHelpers::FObjectFinder<UDataTable> DataTableAsset(TEXT("DataTable'/Game/PirateIsland/Include/Datas/Widget/Inventory/DT_Items.DT_Items'"));
	if (DataTableAsset.Succeeded())
	{
		ItemDataTable = DataTableAsset.Object;
	}
	bTransformTimerUse = true;
}

void ACPickUp::BeginPlay()
{
	Super::BeginPlay();

	InitializePickup(UCItemBase::StaticClass(), ItemQuantity);

	if (this->HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(TransformTimerHandle, this, &ACPickUp::SetTransform, 0.05f, true);
		if (!bTransformTimerUse)
			GetWorld()->GetTimerManager().ClearTimer(TransformTimerHandle);
	}
}

void ACPickUp::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	OnRequestDesroyCalled.Clear(); // CPickUp 파괴되기 전에 델리게이트 바인드한 목록 삭제 
}

// 에디터 내에서 변수 값이 바뀌었을때 델리게이트로 호출됨 (언리얼 Built-in 함수)
void ACPickUp::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(ACPickUp, DesiredItemID)) //바뀐 변수가 DesiredItemID면
	{
		if (ItemDataTable)
		{
			const FString ContextString = DesiredItemID.ToString();

			if (const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString())) //해당 Row가 찾아졌을때
			{
				PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh); // 바뀐 ID에 따라 Mesh Change
			}
		}
	}
}

void ACPickUp::BeginFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(true);
	}
}

void ACPickUp::EndFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(false);
	}
}

void ACPickUp::BeginInteract()
{


}

void ACPickUp::BroadcastUpdatePartialAdded_Implementation(int32 InQuantity)
{
	InteractableData.Quantity = InQuantity;
}

void ACPickUp::RequestDestroy_Implementation()
{
	if (this->HasAuthority())
		Destroy();
}
void ACPickUp::BroadcastDestroy_Implementation()
{
	Destroy();
}



void ACPickUp::BeginFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(true);

	}

}


void ACPickUp::EndFocus()
{

}

void ACPickUp::InitializePickup(const TSubclassOf<class UCItemBase> BaseClass, const int32 InQuantity)
{
	if (ItemDataTable && !DesiredItemID.IsNone()) //Empty String 인지 체크 
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());
		if (ItemData)
		{

			ItemReference = NewObject<UCItemBase>(this, BaseClass);

			ItemReference->ID = ItemData->ID;
			ItemReference->ItemType = ItemData->ItemType;
			ItemReference->NumericData = ItemData->NumericData;
			ItemReference->TextData = ItemData->TextData;
			ItemReference->AssetData = ItemData->AssetData;
			ItemReference->ItemStats = ItemData->ItemStats;
			if (InQuantity <= 0) //0보다 작으면 
			{
				ItemReference->SetQuantity(1);
			}
			else
			{
				ItemReference->SetQuantity(InQuantity);
			}

			PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);

			if (ItemReference->ItemType != EItemType::Build)
			{
				//빌드 아이템이 아닌 경우에는 캐릭터와 통과되도록 설정
				PickupMesh->SetCollisionProfileName(FName("Item"));
				//중력의 영향을 받기 위한 피직스 설정 
				PickupMesh->SetSimulatePhysics(true);
			}
			UpdateInteractableData();
		}
	}
}

void ACPickUp::InitializeDrop(FName ItemID, const int32 InQuantity)
{
	if (this->HasAuthority())
	{
		BroadCastInitializeDrop(ItemID, InQuantity);
	}
	else
	{
		RequestInitializeDrop(ItemID, InQuantity);
	}
}

void ACPickUp::PerformInitializeDrop(UCItemBase* ItemToDrop, const int32 InQuantity)
{
	ItemReference = ItemToDrop;
	InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);
	ItemReference->NumericData.Weight = ItemToDrop->GetItemSingleWeight(); // UCItemBase에서 Item 무게 가져와 설정
	ItemReference->Inventory = nullptr;
	MeshToChange = ItemToDrop->AssetData.Mesh;
	PickupMesh->SetStaticMesh(ItemToDrop->AssetData.Mesh);
	//DT_Items에 DropMesh가 있을 경우 DropMesh로 드롭, 없을 경우 Mesh로 드롭
	if (ItemToDrop->AssetData.DropMesh)
		PickupMesh->SetStaticMesh(ItemToDrop->AssetData.DropMesh);
	else
		PickupMesh->SetStaticMesh(ItemToDrop->AssetData.Mesh);
	PickupMesh->SetCollisionProfileName(FName("Item"));
	PickupMesh->SetSimulatePhysics(true);
	UpdateInteractableData();
}

void ACPickUp::RequestInitializeDrop_Implementation(FName ItemID, const int32 InQuantity)
{
	InitializeDrop(ItemID, InQuantity);
}

void ACPickUp::BroadCastInitializeDrop_Implementation(FName ItemID, const int32 InQuantity)
{
	const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(ItemID, ItemID.ToString());
	if (ItemData)
	{
		UCItemBase* ItemToDrop = NewObject<UCItemBase>(StaticClass());
		ItemToDrop->CopyFromItemData(*ItemData);
		PerformInitializeDrop(ItemToDrop, InQuantity);
	}
}

void ACPickUp::BroadcastSetTransform_Implementation(FTransform InTransform)
{
	SetActorTransform(InTransform);
}

void ACPickUp::BroadcastDestroy_Implementation()
{
	Destroy();
}

void ACPickUp::BroadcastUpdatePartialAdded_Implementation(int32 InQuantity)
{
	InteractableData.Quantity = InQuantity;
}

void ACPickUp::UpdatePartialAdded(int32 InQuantity)
{
	ItemReference->SetQuantity(InQuantity);
	InteractableData.Quantity = InQuantity;
}

void ACPickUp::UpdateInteractableData()
{
	switch (ItemReference->ItemType)
	{
	case EItemType::Build:
		InstanceInteractableData.InteractableType = EInteractableType::Build;
		break;
	default:
		InstanceInteractableData.InteractableType = EInteractableType::Pickup;
		break;
	}
	InstanceInteractableData.Action = ItemReference->TextData.InteractionText;
	InstanceInteractableData.Name = ItemReference->TextData.Name;
	InstanceInteractableData.Quantity = ItemReference->Quantity;
	InstanceInteractableData.ID = ItemReference->ID;
	InteractableData = InstanceInteractableData; // InteractableData 는 인터페이스에서 선언된 FInteractableData
}

void ACPickUp::Interact(ACSurvivor* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		TakePickup(PlayerCharacter);
	}
}

void ACPickUp::TakePickup(const ACSurvivor* Taker)
{
	CDebug::Print("TakePickup");

	if (!IsPendingKillPending()) //픽업 아이템이 Destroy 되고 있는지 체크 
	{
		if (UCInventoryComponent* PlayerInventory = Taker->GetInventoryComponent())
		{
			const FItemAddResult AddResult = PlayerInventory->HandleAddItem(ItemReference);

			switch (AddResult.OperationResult)
			{
			case EItemAddResult::NoItemAdded:
				break;
			case EItemAddResult::PartialItemAdded:
			{
				if (Taker->HasAuthority())
				{
					BroadcastUpdatePartialAdded(ItemReference->Quantity);
				}
				else
				{
					OnUpdatePartialAdded.Broadcast(ItemReference->Quantity);
				}
				UpdateInteractableData(); //PickUp 아이템 수량 조정 
				Taker->GetInteractionComponent()->UpdateInteractionWidget(); //인벤 ui  업뎃
				break;
			}
			case EItemAddResult::AllItemAdded:
			{
				if (Taker->HasAuthority())
				{
					BroadcastDestroy();
				}
				else
				{
					OnRequestDesroyCalled.Broadcast();//RequestDestroy() 호출을 클라이언트에서 하도록 델리게이트로 브로드캐스트 ;
				}
				break;
			}
			}
			CDebug::Print(AddResult.ResultMessage.ToString());
		}
	}
}

void ACPickUp::SetTransform()
{
	BroadcastSetTransform(this->GetActorTransform());
}