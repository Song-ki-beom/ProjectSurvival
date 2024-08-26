#include "ActorComponents/CInventoryComponent.h"
#include "Widget/Inventory/CItemBase.h"
#include "Character/CSurvivorController.h"
#include "Net/UnrealNetwork.h"
#include "Widget/CMainHUD.h"
#include "World/CPickup.h"
#include "Utility/CDebug.h"

UCInventoryComponent::UCInventoryComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

}


void UCInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCInventoryComponent, HUD);


}

void UCInventoryComponent::BeginPlay()
{
	Super::BeginPlay();


	OwnerCharacter = Cast<ACharacter>(GetOwner());

	if (OwnerCharacter)
	{
		ACSurvivorController* playerController = Cast<ACSurvivorController>(OwnerCharacter->GetController());
		if (playerController)
		{
			HUD = Cast<ACMainHUD>(playerController->GetHUD());
		}
	}


	
}

void UCInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

UCItemBase* UCInventoryComponent::FindMatchingItem(UCItemBase* ItemIn) const //ItemIn 존재 여부 확인
{
	if (ItemIn)
	{
		if (InventoryContents.Contains(TWeakObjectPtr<UCItemBase>(ItemIn)))
		{
			return ItemIn;
		}
	}
	return nullptr;
}

UCItemBase* UCInventoryComponent::FindNextItemByID(UCItemBase* ItemIn) const // 일치하는 객체를 찾아서 반환 
{
	if (ItemIn)
	{	
		//ElementType = TArray 의 내부 타입 
		if (const TArray<TWeakObjectPtr<UCItemBase>>::ElementType* Result = InventoryContents.FindByKey(ItemIn))
		{
			return Result->Get();
		}
	}
	return nullptr;
}


UCItemBase* UCInventoryComponent::FindNextPartialStack(UCItemBase* ItemIn) const
{

	//FindByPredicate 와 람다 함수 사용하여 , 캡쳐된 ItemIn 과 InventoryContent 배열의 각 요소를 검사하여 람다함수의 조건식을 만족하는 첫 번째 요소를 찾아 Result 에 반환 
	if (const TArray<TWeakObjectPtr<UCItemBase>>::ElementType* Result = InventoryContents.FindByPredicate(
		[&ItemIn](const TWeakObjectPtr<UCItemBase> InventoryItem)
		{
			return InventoryItem->ID == ItemIn->ID && !InventoryItem->IsFullItemStack(); //ID 일치 , 재고가 최대 허용량인 상태가 아니면  
		}
	))
	{
		return Result->Get(); //찾은 첫번째 요소 반환 
	}

		return nullptr;
}

//슬롯 최대 개수를 신경쓰지 않고, 
//현재 중량을 토대로 Inven에 더할 수 있는 최대 아이템 허용량 계산 
int32 UCInventoryComponent::CalculateWeightAddAmount(UCItemBase* ItemIn, int32 RequestedAddAmount)
{
	const int32 WeightMaxAddAmount = FMath::FloorToInt((GetWeightCapacity() - InventoryTotalWeight) / ItemIn->GetItemSingleWeight()); // 최대 더할수 있는 슬롯의 개수를 계산
	if (WeightMaxAddAmount > RequestedAddAmount)
	{
		return RequestedAddAmount; 
	}
	else
	{
		return WeightMaxAddAmount;
	}


}

//스택 = 아이템 마다 가질 수 있는 최대 재고 개수 
//현재 가지고 있는 스택  갯수를 토대로 추가할 수 있는 최대 스택 개수 계산 
int32 UCInventoryComponent::CalculateNumberForFullStack(UCItemBase* StackableItem, int32 InitialRequestedAddAmount)
{
	const int32 AddAmountToMakeFullStack = StackableItem->NumericData.MaxStackSize - StackableItem->Quantity;

	return FMath::Min(InitialRequestedAddAmount, AddAmountToMakeFullStack);
}


//처음에 아이템을 받았을 때 여기서 처리 
FItemAddResult UCInventoryComponent::HandleAddItem(class UCItemBase* InItem)
{
	if (GetOwner())
	{
		const int32 InitialRequestedAddAmount = InItem->Quantity;
		//Stack 가능한 아이템이 아닐 때, 기존에 아이템이 없음
		if (!InItem->NumericData.bIsStackable)
		{
			return HandleNonStackableItems(InItem, InitialRequestedAddAmount);
		}
		
			//Stack 가능한 아이템일 때, 기존에 Item이 존재하고 Stack 가능 개수가 남아있음 
			const int32 StackableAmountAdded = HandleStackableItems(InItem, InitialRequestedAddAmount);

			//StackableAmountAdded 결과에 따라 분기 처리 


			if (StackableAmountAdded == InitialRequestedAddAmount)
			{
				return FItemAddResult::AddedAll(InitialRequestedAddAmount, FText::Format(FText::FromString("{0} {1} All added To Inventory"),InItem->TextData.Name, InitialRequestedAddAmount));
			}

			//Stack 한 결과가 처음 리퀘스트한 양이 아닌 일부만 넣어졌을때   
			if (StackableAmountAdded < InitialRequestedAddAmount && StackableAmountAdded >0)
			{
				return FItemAddResult::AddedPartial(InitialRequestedAddAmount, FText::Format(FText::FromString("{0} x {1} Partial Amount of Item Added"), InItem->TextData.Name, StackableAmountAdded));
			}

			if (StackableAmountAdded <= 0)
			{
				return FItemAddResult::AddedNone(FText::Format(FText::FromString("Cannot Add {0} Due To Inventory Weight or Slot Capacity"), InItem->TextData.Name));
			}



		}

	check(false);
	return FItemAddResult::AddedNone(FText::FromString("HandleItem Fail Due To OwnerCharacter Problem"));

	
}

void UCInventoryComponent::ToggleMenu()
{
	if (HUD)
		HUD->ToggleMenu();
}

void UCInventoryComponent::DropItem(UCItemBase* ItemToDrop, const int32 QuantityToDrop)
{
	if (FindMatchingItem(ItemToDrop))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = OwnerCharacter;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn; // 벽에 끼이는 등  spawn 에 실패하는 상황이 생기면 위치를 Adjust해서 성공시킴 
		 
		//Drop할 아이템 Spawn Location 과 Transform 설정 
		const FVector SpawnLocation =  OwnerCharacter->GetActorLocation() + (OwnerCharacter->GetActorForwardVector() * 50.0f) ;
		 const FTransform SpawnTransform(OwnerCharacter->GetActorRotation(), SpawnLocation);
		
		 //QuantityToDrop 만큼 인벤토리에서 제거 
		 const int32 RemovedQuantity = RemoveAmountOfItem(ItemToDrop, QuantityToDrop); 

		 ACPickUp* Pickup = GetWorld()->SpawnActor<ACPickUp>(ACPickUp::StaticClass(), SpawnTransform, SpawnParams); //PickUp 클래스는 CObject Construct 이후에 , 의도적으로 Initialize를 하거나 PickUp 이벤트가 일어날때 액터의 재복사 및 전달 데이터의 Copy가 일어나기 때문에 StaticClass 로 정적 생성해도 괜찮다.

		 Pickup->InitializeDrop(ItemToDrop, RemovedQuantity);
	}
	else
	{
		CDebug::Print(TEXT("No Matching Item Do Drop ! Failed Drop"));
	}

}

//IsStackable 이 False 인 데이터 수납할 시, 중첩 불가 단일 개수 
FItemAddResult UCInventoryComponent::HandleNonStackableItems(UCItemBase* ItemIn, int32 RequestedAddAmount)
{
	{//return none check

		//중량이 없으면 -> 아이템 정보가 이상한지 체크 
		if (FMath::IsNearlyZero(ItemIn->GetItemStackWeight()) || ItemIn->GetItemStackWeight() < 0)
		{
			return FItemAddResult::AddedNone(FText::Format(FText::FromString("{0} Item Info Invalid. Cannot put Into Inventory"), ItemIn->TextData.Name));
		}

		//아이템이 최대 허용 용량을 넘는다면 
		if (InventoryTotalWeight + ItemIn->GetItemSingleWeight() > GetWeightCapacity())
		{
			return FItemAddResult::AddedNone(FText::FromString("Over Max Weight. Cannot put Into Inventory"));
		}

		//아이템 슬롯 추가로 최대 슬롯 갯수를 넘으면 
		if (InventoryContents.Num() + 1 > InventorySlotsCapacity)
		{
			return FItemAddResult::AddedNone(FText::Format(FText::FromString("Over Max Slot. Cannot Put Into Inventory"), ItemIn->TextData.Name));
		}



	}
	AddNewItem(ItemIn, 1);
	return FItemAddResult::AddedAll(RequestedAddAmount,FText::Format(FText::FromString("Successfully put  {0} X{1} Item into Inventoty"), ItemIn->TextData.Name, RequestedAddAmount));

}



int32 UCInventoryComponent::HandleStackableItems(UCItemBase*, int32 RequestedAddAmount)
{

	return 1;
}






void UCInventoryComponent::AddNewItem(UCItemBase* InItem, const int32 AmountToAdd)
{
	UCItemBase* NewItem;

	//Copy .. 두군데에서의 같은 메모리 참조 방지
	if (InItem->bIsCopy || InItem->bIsPickup) //World 상의 Item이 참조하는 메모리 주소와 NewItem의 참조 주소가 곂치지 않기 위해 별도의 Copy 인지, PickUp되어 사라질 데이터인지 체크 
	{
		
		NewItem = InItem;
		NewItem->ResetItemFlags();
	}
	else
	{
		//World 에서 오는 Item을 참조하는거면 새로 Copy , 또는 아이템을  Split 해야 할 상황이면 Copy
		NewItem = InItem->CreateItemCopy();
	}

	//아이템 세팅
	NewItem->Inventory = this;
	NewItem->SetQuantity(AmountToAdd);
	InItem->GetItemStackWeight();
	NewItem->GetItemStackWeight();
	//인벤 TArray에 추가 
	InventoryContents.Add(NewItem);
	InventoryTotalWeight += NewItem->GetItemStackWeight();
	OnInventoryUpdated.Broadcast();
}




void UCInventoryComponent::RemoveSingleItem(UCItemBase* ItemIn)
{
	InventoryContents.RemoveSingle(ItemIn);
	OnInventoryUpdated.Broadcast();
}

int32 UCInventoryComponent::RemoveAmountOfItem(UCItemBase* ItemIn, int32 DesiredAmountToRemove)
{
	const int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove, ItemIn->Quantity);

	ItemIn->SetQuantity(ItemIn->Quantity - ActualAmountToRemove);

	InventoryTotalWeight -= ActualAmountToRemove * ItemIn->GetItemSingleWeight();

	OnInventoryUpdated.Broadcast();

	return  ActualAmountToRemove;

}

void UCInventoryComponent::SplitExistingStack(UCItemBase* ItemIn, const int32 AmountToSplit)
{
	//사용 가능 슬롯이 남아있으면
	if (!(InventoryContents.Num() + 1 > InventorySlotsCapacity))
	{
		RemoveAmountOfItem(ItemIn, AmountToSplit);
		AddNewItem(ItemIn, AmountToSplit);
	}

}


