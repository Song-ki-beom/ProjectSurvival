// Fill out your copyright notice in the Description page of Project Settings.


#include "World/CPickUp.h"
#include "Character/CSurvivor.h"
#include "ActorComponents/CInventoryComponent.h"
#include "Utility/CDebug.h"
#include "ActorComponents/CInteractionComponent.h"
#include "Widget/Inventory/CItemBase.h"
// Sets default values
ACPickUp::ACPickUp()
{
	PrimaryActorTick.bCanEverTick = false;
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	PickupMesh->SetSimulatePhysics(false);
	SetRootComponent(PickupMesh);

}

void ACPickUp::BeginPlay()
{
	Super::BeginPlay();

	InitializePickup(UCItemBase::StaticClass(), ItemQuantity);
	
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

void ACPickUp::Interact(class ACSurvivor* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		TakePickup(PlayerCharacter);

	}
}



void ACPickUp::TakePickup(const ACSurvivor* Taker)
{
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
				UpdateInteractableData(); //PickUp 아이템 수량 조정 
					Taker->GetInteractionComponent()-> UpdateInteractionWidget(); //인벤 ui  업뎃
				break;
			case EItemAddResult::AllItemAdded:
				Destroy();
				break;
			}

			CDebug::Print(AddResult.ResultMessage.ToString());
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


void ACPickUp::InitializePickup(const TSubclassOf<class UCItemBase> BaseClass, const int32 InQuantity)
{
	if (ItemDataTable && !DesiredItemID.IsNone()) //Empty String 인지 체크 
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());

		ItemReference = NewObject<UCItemBase>(this, BaseClass);

		ItemReference->ID = ItemData->ID;
		ItemReference->ItemType = ItemData->ItemType;
		ItemReference->NumericData = ItemData->NumericData;
		ItemReference->TextData = ItemData->TextData;
		ItemReference->AssetData = ItemData->AssetData;

		InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);

		PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);

		UpdateInteractableData();
	}
}

void ACPickUp::InitializeDrop(UCItemBase* ItemToDrop, const int32 InQuantity)
{
	ItemReference = ItemToDrop;
	InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);
	ItemReference->NumericData.Weight = ItemToDrop->GetItemSingleWeight(); // UCItemBase에서 Item 무게 가져와 설정
	PickupMesh->SetStaticMesh(ItemToDrop->AssetData.Mesh);
	UpdateInteractableData();

}


void ACPickUp::UpdateInteractableData()
{
	InstanceInteractableData.InteractableType = EInteractableType::Pickup;
	InstanceInteractableData.Action = ItemReference->TextData.InteractionText;
	InstanceInteractableData.Name = ItemReference->TextData.Name;
	InstanceInteractableData.Quantity = ItemReference->Quantity;
	InteractableData = InstanceInteractableData; // InteractableData 는 인터페이스에서 선언된 FInteractableData




}




void ACPickUp::BeginInteract()
{

}

void ACPickUp::EndInteract()
{

}
