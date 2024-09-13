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

void ACStructure_Placeable::PerformAddItem(FName InID, int32 InQuantity, FItemNumericData InNumericData)
{
	FItemInformation addedItemInfo;
	addedItemInfo.ItemID = InID;
	addedItemInfo.Quantity = InQuantity;
	addedItemInfo.NumericData = InNumericData;
	addedItemInfo.InventoryIndex = WidgetRefreshTrigger;
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
				PerformAddItem(addedItemInfo.ItemID, addedItemInfo.Quantity - addQuantity, addedItemInfo.NumericData);
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
