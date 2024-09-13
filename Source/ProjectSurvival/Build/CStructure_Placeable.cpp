#include "Build/CStructure_Placeable.h"
#include "Build/CStructure_Foundation.h"
#include "Build/CStructure_Ceiling.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ActorComponents/CActorInventoryComponent.h"
#include "Widget/CMainHUD.h"
#include "Character/CSurvivor.h"
#include "Character/CSurvivorController.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"
#include "Widget/Inventory/CItemBase.h"
#include "Widget/Inventory/CInventoryPanel_WorkingBench.h"
#include "ActorComponents/CInventoryComponent.h"
#include "Utility/CDebug.h"

ACStructure_Placeable::ACStructure_Placeable()
{
	//bReplicates = true;

	DownBox = CreateDefaultSubobject<UBoxComponent>("DownBox");
	DownBox->SetupAttachment(PickupMesh);
}

void ACStructure_Placeable::BeginPlay()
{
	Super::BeginPlay();
	ActorInventoryWidget = CreateWidget<UUserWidget>(GetWorld(), ActorInventoryWidgetClass);
	WorkingBenchWidget = Cast<UCInventoryPanel_WorkingBench>(ActorInventoryWidget);
	if (WorkingBenchWidget)
		WorkingBenchWidget->SetOwnerActor(this);
}

void ACStructure_Placeable::CheckDown_FoundationAndCeiling()
{
	// 액터 체크
	FHitResult actorHitResult;
	FVector startLocation = PreviewBox->GetComponentLocation();
	FVector endLocation = startLocation - FVector(0, 0, 300);
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypeQuery;
	objectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECC_Visibility));
	TArray<AActor*> ignores;
	ignores.Add(this);
	FCollisionQueryParams collisionParams;
	bool bDownActorHit = UKismetSystemLibrary::LineTraceSingleForObjects
	(
		GetWorld(),
		startLocation,
		endLocation,
		objectTypeQuery,
		false,
		ignores,
		EDrawDebugTrace::ForDuration,
		actorHitResult,
		true,
		FLinearColor::Yellow,
		FLinearColor::Red
	);

	if (bDownActorHit) // 라인트레이스가 어떤 액터에 히트했는지 확인
	{
		AActor* hitActor = actorHitResult.GetActor();
		if (hitActor && (hitActor->IsA(ACStructure_Foundation::StaticClass()) || hitActor->IsA(ACStructure_Ceiling::StaticClass())))
		{
			//CDebug::Print("hitActor: ", hitActor, FColor::Cyan);
			bDown_FoundationAndCeilingActorHit = true;
			PlaceableHeight = actorHitResult.ImpactPoint.Z;
		}
		else
		{

		}
	}
	else
	{
		bDown_FoundationAndCeilingActorHit = false;
		PlaceableHeight = GetOwner()->GetActorLocation().Z;
	}
}

void ACStructure_Placeable::CheckCenter()
{
	FHitResult centerBoxHitResult;
	FVector centerBoxLocation = PreviewBox->GetComponentLocation();
	FVector centerBoxSize = PreviewBox->GetScaledBoxExtent() - FVector(0, 0, 1);
	FRotator centerBoxOrientation;
	centerBoxOrientation = this->GetActorRotation();
	ETraceTypeQuery centerBoxTraceTypeQuery = ETraceTypeQuery::TraceTypeQuery2;
	bool bCenterBoxTraceComplex = false;
	TArray<AActor*> centerBoxActorsToIgnore;
	TArray<FHitResult> centerBoxHitResults;
	bCenterHit = UKismetSystemLibrary::BoxTraceSingle(
		GetWorld(),
		centerBoxLocation,
		centerBoxLocation,
		centerBoxSize,
		centerBoxOrientation,
		centerBoxTraceTypeQuery,
		bCenterBoxTraceComplex,
		centerBoxActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		centerBoxHitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red
	);
}

void ACStructure_Placeable::OpenActorInventory(const ACSurvivor* Survivor, class AActor* Actor)
{
	//CDebug::Print("OpenActorInventory Called");
	if (Survivor)
	{
		switch (WidgetCaller)
		{
		case EWidgetCall::WorkBench:
		{
			if (ActorInventoryWidgetClass)
			{
				if (ActorInventoryWidget)
				{
					ACSurvivorController* survivorController = Cast<ACSurvivorController>(Survivor->GetController());
					if (survivorController)
					{
						ACMainHUD* mainHUD = Cast<ACMainHUD>(survivorController->GetHUD());
						if (mainHUD)
						{
							mainHUD->SetWidgetVisibility(EWidgetCall::WorkBench, ActorInventoryWidget, Actor);
						}
					}
				}
				else
					CDebug::Print("ActorInventoryWidget is not Valid");
			}
			else
				CDebug::Print("ActorInventoryWidgetClass is not Valid");
		}
		}
	}
	else
		CDebug::Print("Survivor is not valid");
}

void ACStructure_Placeable::PerformAddItem(FName InID, int32 InQuantity, FItemNumericData InNumericData, EItemType InItemType)
{
	FItemInformation addedItemInfo;
	addedItemInfo.ItemID = InID;
	addedItemInfo.Quantity = InQuantity;
	addedItemInfo.NumericData = InNumericData;
	addedItemInfo.ItemType = InItemType;
	// 스택이 가능한 아이템인지 검사
	if (addedItemInfo.NumericData.bIsStackable)
	{
		// 최대 스택이 아니면서 ID가 같은 인덱스를 반환, 못 찾을 경우 -1 반환
		int32 resultIndex = GetIndexOfNonFullStackByID(addedItemInfo);
		if (resultIndex == -1)
			ItemInfoArray.Add(addedItemInfo);
		else
		{
			// 더했을 때 최대 스택을 초과하는지 검사
			if (CheckMaxStack(addedItemInfo, resultIndex))
			{
				int32 addQuantity = ItemInfoArray[resultIndex].NumericData.MaxStackSize - ItemInfoArray[resultIndex].Quantity;
				ItemInfoArray[resultIndex].Quantity += addQuantity;

				// 최대 스택만큼 더하고 남은 양만큼 다시 PerformAddItem 호출
				PerformAddItem(addedItemInfo.ItemID, addedItemInfo.Quantity - addQuantity, addedItemInfo.NumericData ,addedItemInfo.ItemType);
			}
			else
				ItemInfoArray[resultIndex].Quantity += addedItemInfo.Quantity;
		}
	}
	else
		ItemInfoArray.Add(addedItemInfo);

	// 클라이언트에서 AddItemInfoToWidget() 호출하기위한 OnRep_WidgetRefreshTrigger 트리거
	WidgetRefreshTrigger++;

	// TArray는 리플리케이트 되더라도 특정 인덱스의 내용물만 바꾸는 것으로는 리플리케이트 되지 않음
	// 새로 초기화 해주거나 행렬 원소 갯수가 변경되어야 리플리케이트가 적용됨
	SharedItemInfoArray = ItemInfoArray;

	AddItemInfoToWidget();
}

void ACStructure_Placeable::PerformRemoveItem(FName InID, int32 InQuantity, FItemNumericData InNumericData)
{
	
}

void ACStructure_Placeable::PerformSwapItem(int32 idxBase, int32 idxDrag)
{
	ItemInfoArray.Swap(idxBase, idxDrag);
	SharedItemInfoArray = ItemInfoArray;
	AddItemInfoToWidget();
	WidgetRefreshTrigger++;

}

void ACStructure_Placeable::PerformCombineItem(int32 idxBase, int32 idxDrag)
{
	int32 AmountLeftToDistribute = ItemInfoArray[idxDrag].Quantity;
	const int32 AmountToMakeFullStack = ItemInfoArray[idxBase].NumericData.MaxStackSize - ItemInfoArray[idxBase].Quantity; //남은 수납가능 재고량 계산 
	if (AmountToMakeFullStack >= AmountLeftToDistribute) //재고량이 넘치면 다 넣고 드래그 아이템 삭제
	{
		ItemInfoArray[idxBase].Quantity += AmountLeftToDistribute;
		ItemInfoArray.RemoveAt(idxDrag);

	}
	else if (AmountToMakeFullStack < AmountLeftToDistribute) // 다 넣을수 없으면 일부만 넣기 
	{
		//기존 인벤 아이템에 add
		ItemInfoArray[idxBase].Quantity += AmountToMakeFullStack;

		//넣은 개수만큼 기존의 빼오는 아이템 재고에 Subtract 
		AmountLeftToDistribute -= AmountToMakeFullStack;
		ItemInfoArray[idxDrag].Quantity = AmountLeftToDistribute;
	}
	SharedItemInfoArray = ItemInfoArray;
	AddItemInfoToWidget();
	WidgetRefreshTrigger++;
}

void ACStructure_Placeable::PerformSplitItem(int32 ItemIdx, int32 AmountToSplit)
{
	FItemInformation addedItemInfo;
	addedItemInfo.ItemID = ItemInfoArray[ItemIdx].ItemID;
	addedItemInfo.Quantity = AmountToSplit;
	addedItemInfo.NumericData = ItemInfoArray[ItemIdx].NumericData;
	addedItemInfo.ItemType = ItemInfoArray[ItemIdx].ItemType;

	// 스택이 가능한 아이템인지 검사
	if (addedItemInfo.NumericData.bIsStackable)
	{

		ItemInfoArray[ItemIdx].Quantity -= AmountToSplit;
		ItemInfoArray.Add(addedItemInfo);
		SharedItemInfoArray = ItemInfoArray;
		AddItemInfoToWidget();
		WidgetRefreshTrigger++;

	}
}

		


	



void ACStructure_Placeable::AddItemInfoToWidget()
{
	// 추가하기 전 UCItemBase 배열 초기화
	ActorInventoryContents.Empty();

	// 리플리케이트된 FItemData의 배열 SharedItemInfoArray를 기반으로 UCItemBase 생성
	for (int32 tempIndex = 0; tempIndex < SharedItemInfoArray.Num(); tempIndex++)
	{
		FName tempID = SharedItemInfoArray[tempIndex].ItemID;
		FItemData* itemData = ItemDataTable->FindRow<FItemData>(tempID, TEXT(""));
		if (itemData)
		{
			UCItemBase* ItemCopy = NewObject<UCItemBase>(StaticClass());
			ItemCopy->ID = tempID;
			ItemCopy->Quantity = SharedItemInfoArray[tempIndex].Quantity;
			ItemCopy->ItemType = itemData->ItemType;
			ItemCopy->TextData = itemData->TextData;
			ItemCopy->ItemStats = itemData->ItemStats;
			ItemCopy->NumericData = itemData->NumericData;
			ItemCopy->AssetData = itemData->AssetData;
			ItemCopy->bIsCopy = true;

			ActorInventoryContents.Add(ItemCopy);

			switch (WidgetCaller)
			{
			case EWidgetCall::WorkBench:
			{
				UCInventoryPanel_WorkingBench* workingBenchWidget = Cast<UCInventoryPanel_WorkingBench>(ActorInventoryWidget);
				if (workingBenchWidget)
				{
					// workingBenchWidget의 클래스에 UCItemBase의 배열 전달
					workingBenchWidget->SetWidgetItems(ActorInventoryContents);
					workingBenchWidget->RefreshWorkingBenchInventory();
				}
			}
			}
		}
		else
			CDebug::Print("Itemdata is not Valid", FColor::Magenta);
	}
}

void ACStructure_Placeable::OnRep_WidgetRefreshTrigger()
{
	AddItemInfoToWidget();
}

int32 ACStructure_Placeable::GetIndexOfNonFullStackByID(const FItemInformation InItemInformation)
{
	for (int32 index = 0; index < ItemInfoArray.Num(); ++index)
	{
		const FItemInformation& checkItem = ItemInfoArray[index];
		if ((checkItem.ItemID == InItemInformation.ItemID) && (checkItem.Quantity < checkItem.NumericData.MaxStackSize))
		{
			return index;
		}
	}
	return -1;
}

bool ACStructure_Placeable::CheckMaxStack(const FItemInformation InItemInformation, const int32 InIndex)
{
	return ItemInfoArray[InIndex].Quantity + InItemInformation.Quantity > ItemInfoArray[InIndex].NumericData.MaxStackSize;
}

void ACStructure_Placeable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACStructure_Placeable, WidgetRefreshTrigger);
	DOREPLIFETIME(ACStructure_Placeable, SharedItemInfoArray);
}



//타입 , 수량 순으로 정렬 
void ACStructure_Placeable::PerformSortInfoWidget()
{
	if (ItemInfoArray.Num() == 0) return;


	//병합 정렬 사용 
	MergeSort(ItemInfoArray, 0, ItemInfoArray.Num() - 1);
	SharedItemInfoArray = ItemInfoArray;
	AddItemInfoToWidget();
	WidgetRefreshTrigger++;
}


void ACStructure_Placeable::Merge(TArray<FItemInformation>& Array, int Left, int Mid, int Right)
{
	int n1 = Mid - Left + 1; //왼쪽 분할부분 원소 갯수
	int n2 = Right - Mid; // 오른쪽 분할부분 원소 갯수

	TArray<FItemInformation> LeftArray;
	TArray<FItemInformation> RightArray;

	for (int i = 0; i < n1; i++)
		LeftArray.Add(Array[Left + i]);
	for (int i = 0; i < n2; i++)
		RightArray.Add(Array[Mid + 1 + i]);


	//병합 시작 
	int i = 0, j = 0, k = Left;
	while (i < n1 && j < n2)
	{
		int32 LeftIDNumber = LeftArray[i].ItemID.GetNumber(); //GetNumber : 아이디(FName)의 적힌 숫자를 가져옴 
		int32 RightIDNumber = RightArray[j].ItemID.GetNumber();

		if (LeftArray[i].ItemType < RightArray[j].ItemType ||
			(LeftArray[i].ItemType == RightArray[j].ItemType && LeftArray[i].Quantity < RightArray[j].Quantity)
			|| (LeftArray[i].ItemType == RightArray[j].ItemType && LeftArray[i].Quantity == RightArray[j].Quantity && LeftIDNumber >= RightIDNumber)) //아이템 타입 Enum 내림차순 , Quantity 내림차순 , 아이템 ID 오름차순으로 정렬 
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

void ACStructure_Placeable::MergeSort(TArray<FItemInformation>& Array, int Left, int Right)
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


