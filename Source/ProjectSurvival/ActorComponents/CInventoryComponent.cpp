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

//Max스택 = 아이템 마다 가질 수 있는 최대 재고 개수 
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
			if (StackableAmountAdded <= 0)
			{
				return FItemAddResult::AddedNone(FText::Format(FText::FromString("Cannot Add {0} Due To Inventory Weight or Slot Capacity"), InItem->TextData.Name));
			}

			//EarnedItem UI 표시
			UCItemBase* EarnedItem = InItem->CreateItemCopy();
			EarnedItem->Quantity = StackableAmountAdded;
			HUD->AddEarnedInfo(EarnedItem);

			if (StackableAmountAdded == InitialRequestedAddAmount)
			{
				return FItemAddResult::AddedAll(InitialRequestedAddAmount, FText::Format(FText::FromString("{0} {1} All added To Inventory"),InItem->TextData.Name, InitialRequestedAddAmount));
				
			}

			//Stack 한 결과가 처음 리퀘스트한 양이 아닌 일부만 넣어졌을때   
			if (StackableAmountAdded < InitialRequestedAddAmount && StackableAmountAdded >0)
			{
				return FItemAddResult::AddedPartial(InitialRequestedAddAmount, FText::Format(FText::FromString("{0} x {1} Partial Amount of Item Added"), InItem->TextData.Name, StackableAmountAdded));
			}

			



		}

	check(false);
	return FItemAddResult::AddedNone(FText::FromString("HandleItem Fail Due To OwnerCharacter Problem"));

	
}

//void UCInventoryComponent::ToggleMenu()
//{
//	//if (HUD)
//	//	HUD->ToggleMenu();
//}






void UCInventoryComponent::DropItem(UCItemBase* ItemToDrop, const int32 QuantityToDrop)
{
	if (FindMatchingItem(ItemToDrop))
	{
		
		 
		//Drop할 아이템 Spawn Location 과 Transform 설정 
		const FVector SpawnLocation =  OwnerCharacter->GetActorLocation() + (OwnerCharacter->GetActorForwardVector() * 50.0f) ;
		const FTransform SpawnTransform(OwnerCharacter->GetActorRotation(), SpawnLocation);
		
		 //QuantityToDrop 만큼 인벤토리에서 제거 
		 const int32 RemovedQuantity = RemoveAmountOfItem(ItemToDrop, QuantityToDrop); 
		 
		// FItemData ItemToDropData = ItemToDrop->CreateFItemData(ItemToDrop);
		 FName ItemID = ItemToDrop->ID;
		 if (OwnerCharacter->HasAuthority())
		 {
			 PerformDropItem(SpawnTransform, ItemID, RemovedQuantity);
		 }
		 else
		 {
			 RequestDropItem(SpawnTransform, ItemID, RemovedQuantity);
		 }
			 
	}
	else
	{
		CDebug::Print(TEXT("No Matching Item Do Drop ! Failed Drop"));
	}

}

void UCInventoryComponent::PerformDropItem( const  FTransform SpawnTransform, FName ItemID,  const int32 RemovedQuantity)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn; // 벽에 끼이는 등  spawn 에 실패하는 상황이 생기면 위치를 Adjust해서 성공시킴 


	ACPickUp* Pickup = GetWorld()->SpawnActor<ACPickUp>(ACPickUp::StaticClass(), SpawnTransform, SpawnParams); 
	Pickup->InitializeDrop(ItemID, RemovedQuantity);

	
}

void UCInventoryComponent::RequestDropItem_Implementation(const  FTransform SpawnTransform, FName ItemID, const int32 RemovedQuantity)
{

	if (OwnerCharacter->HasAuthority())
	{
		PerformDropItem(SpawnTransform, ItemID, RemovedQuantity);
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



int32 UCInventoryComponent::HandleStackableItems(UCItemBase* ItemIn, int32 RequestedAddAmount)
{
	if (RequestedAddAmount <= 0 || FMath::IsNearlyZero(ItemIn->GetItemStackWeight()))
	{
		return 0;
	}

	int32 AmountLeftToDistribute = RequestedAddAmount;
	UCItemBase* ExistingItem = FindNextPartialStack(ItemIn); //인벤에 일치하는 아이템 있는지 확인 
	
	while (ExistingItem) //이미 인벤에 같은 아이템 종류가 있으면 
	{
		const int32 AmountToMakeFullStack = CalculateNumberForFullStack(ExistingItem, AmountLeftToDistribute); //넣을수 있는 최대 재고량 계산 (1차 필터)
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ExistingItem, AmountToMakeFullStack); //허용 가능한 최대 중량을 기반으로  넣을 수 있는 최대 재고량 계산 (2차 필터)

		if (WeightLimitAddAmount > 0)
		{
			//기존 인벤 아이템에 add
			ExistingItem->SetQuantity(ExistingItem->Quantity + WeightLimitAddAmount);
			InventoryTotalWeight += ExistingItem->GetItemSingleWeight() * WeightLimitAddAmount;
			
			
			//넣은 개수만큼 기존의 빼오는 아이템 재고에 Subtract 
			AmountLeftToDistribute -= WeightLimitAddAmount; 
			ItemIn->SetQuantity(AmountLeftToDistribute);


			if (InventoryTotalWeight + ExistingItem->GetItemSingleWeight()> InventoryWeightCapacity) //무게 초과 시
			{
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountLeftToDistribute; //넣은 재고량 반환 
			}

		}


		else if (WeightLimitAddAmount <= 0) //무게 문제로 넣지 못할 시
		{
			if (AmountLeftToDistribute != RequestedAddAmount)
			{
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountLeftToDistribute; 
			}

			//AmountLeftToDistribute == RequestedAddAmount 일 시(넣은게 한개도 없음) 
			return 0; 
		}

		if (AmountLeftToDistribute <= 0) //모든 Request재고량을 넣었을 시 
		{
			OnInventoryUpdated.Broadcast();
			return RequestedAddAmount;
		}

		ExistingItem = FindNextPartialStack(ItemIn); //while문을 돌며 또 다른 넣을 수 있는 재고(Slot)가 있나 체크  
	} 


	//기존 Item에 추가 완료 후, 아직 AmountLeftToDistributes가 남았으면,
	//새로 Slot을 생성함.. 여분의 슬롯 개수가 있는지 체크 
	if (InventoryContents.Num() + 1 <= InventorySlotsCapacity)
	{
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ItemIn, AmountLeftToDistribute);

		if (WeightLimitAddAmount > 0)
		{
			if (WeightLimitAddAmount < AmountLeftToDistribute) //새로 만들긴 하지만 넣을수 있는 재고가 요청만큼 안나올때
			{
				AmountLeftToDistribute -= WeightLimitAddAmount;
				ItemIn->SetQuantity(AmountLeftToDistribute);

				AddNewItem(ItemIn->CreateItemCopy(), WeightLimitAddAmount);
				return RequestedAddAmount - AmountLeftToDistribute;

			}

			//새로운 아이템 Slot 에 모두 넣을수 있는 상태일때
			AddNewItem(ItemIn->CreateItemCopy(), AmountLeftToDistribute);
			return RequestedAddAmount;
		}

		//아이템 슬롯 여유 but WeightLimitAddAmount 0 일때(더 추가할 필요가 없을 때)
		return RequestedAddAmount - AmountLeftToDistribute;
	}

	//no Capacity , no Slot
	return 0;

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
	NewItem->ItemStats.DamageValue;
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

bool UCInventoryComponent::SplitExistingStack(UCItemBase* ItemIn, const int32 AmountToSplit)
{
	//사용 가능 슬롯이 남아있으면
	if (ItemIn->Quantity - AmountToSplit <= 0) return false;
	if (!(InventoryContents.Num() + 1 > InventorySlotsCapacity))
	{
		RemoveAmountOfItem(ItemIn, AmountToSplit);
		AddNewItem(ItemIn, AmountToSplit);
		return true;
	}
	
		return false;

}


//타입 , 수량 순으로 정렬 
void UCInventoryComponent::SortInventory()
{
	if (InventoryContents.Num() == 0) return;

	//병합 정렬 사용 
	MergeSort(InventoryContents, 0, InventoryContents.Num() - 1);




	OnInventoryUpdated.Broadcast();
}


void UCInventoryComponent::Merge(TArray<TWeakObjectPtr<UCItemBase>>& Array, int Left, int Mid, int Right)
{
	int n1 = Mid - Left + 1; //왼쪽 분할부분 원소 갯수
	int n2 = Right - Mid; // 오른쪽 분할부분 원소 갯수

	TArray<TWeakObjectPtr<UCItemBase>> LeftArray;
	TArray<TWeakObjectPtr<UCItemBase>> RightArray;

	for (int i = 0; i < n1; i++)
		LeftArray.Add(Array[Left + i]);
	for (int i = 0; i < n2; i++)
		RightArray.Add(Array[Mid + 1 + i]);


	//병합 시작 
	int i = 0, j = 0, k = Left;
	while (i < n1 && j < n2)
	{
		int32 LeftIDNumber = LeftArray[i]->ID.GetNumber(); //GetNumber : 아이디(FName)의 적힌 숫자를 가져옴 
		int32 RightIDNumber = RightArray[j]->ID.GetNumber();

		if (LeftArray[i]->ItemType < RightArray[j]->ItemType ||
			(LeftArray[i]->ItemType == RightArray[j]->ItemType && LeftArray[i]->Quantity < RightArray[j]->Quantity)
			||(LeftArray[i]->ItemType == RightArray[j]->ItemType && LeftArray[i]->Quantity == RightArray[j]->Quantity&& LeftIDNumber >= RightIDNumber)) //아이템 타입 Enum 내림차순 , Quantity 내림차순 , 아이템 ID 오름차순으로 정렬 
		{
			Array[k] = RightArray[j];
			j++;
		}
		else
		{
			Array[k] = LeftArray[i];
			i++;
		}
		k++;
	}

	// 남은 값 복사
	while (i < n1)
	{
		Array[k] = LeftArray[i];
		i++;
		k++;
	}

	while (j < n2)
	{
		Array[k] = RightArray[j];
		j++;
		k++;
	}
}

void UCInventoryComponent::MergeSort(TArray<TWeakObjectPtr<UCItemBase>>& Array, int Left, int Right)
{
	if (Left < Right)
	{
		int Mid = Left + (Right - Left) / 2;

		// 좌우 분할
		MergeSort(Array, Left, Mid);
		MergeSort(Array, Mid + 1, Right);

		// 병합
		Merge(Array, Left, Mid, Right);
	}
}