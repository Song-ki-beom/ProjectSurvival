#include "Build/CStructure_Placeable.h"
#include "Build/CStructure_Foundation.h"
#include "Build/CStructure_Ceiling.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ActorComponents/CActorInventoryComponent.h"
#include "Character/CSurvivor.h"
#include "Character/CSurvivorController.h"
#include "Blueprint/UserWidget.h"
#include "ActorComponents/CInventoryComponent.h"
#include "Widget/CMainHUD.h"
#include "Widget/Inventory/CItemBase.h"
#include "Widget/Inventory/CInventoryPanel_Placeable.h"
#include "Widget/Produce/CProduceWidget.h"
#include "Widget/Produce/CProduceItemQueueSlot.h"
#include "Net/UnrealNetwork.h"
#include "Utility/CDebug.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "LandScape.h"

ACStructure_Placeable::ACStructure_Placeable()
{
	//bReplicates = true;

	DownBox = CreateDefaultSubobject<UBoxComponent>("DownBox");
	DownBox->SetupAttachment(PickupMesh);
}

void ACStructure_Placeable::BeginPlay()
{
	Super::BeginPlay();

	switch (WidgetCaller)
	{
	case EWidgetCall::Placeable:
		if (ActorInventoryWidgetClass)
		{
			ActorInventoryWidget = CreateWidget<UUserWidget>(GetWorld(), ActorInventoryWidgetClass);
			PlaceableWidget = Cast<UCInventoryPanel_Placeable>(ActorInventoryWidget);
			if (PlaceableWidget)
			{
				PlaceableWidget->SetOwnerActor(this);

				switch (PlaceableStructureType)
				{
				case EPlaceableStructureType::WorkingBench:
					PlaceableWidget->SetInventoryWindowName(FText::FromString(TEXT("인벤토리 - 작업대")));
					break;
				case EPlaceableStructureType::Furnace:
					PlaceableWidget->SetInventoryWindowName(FText::FromString(TEXT("인벤토리 - 화로")));
					break;
				case EPlaceableStructureType::CampFire:
					PlaceableWidget->SetInventoryWindowName(FText::FromString(TEXT("인벤토리 - 모닥불")));
					break;
				default:
					break;
				}
			}
		}
		
		if (ActorProduceWidgetClass)
		{
			ActorProduceWidget = CreateWidget<UUserWidget>(GetWorld(), ActorProduceWidgetClass);
			PlaceableProduceWidget = Cast<UCProduceWidget>(ActorProduceWidget);
			if (PlaceableProduceWidget)
			{
				PlaceableProduceWidget->SetOwnerActor(this, WidgetCaller);

				switch (PlaceableStructureType)
				{
				case EPlaceableStructureType::WorkingBench:
					PlaceableProduceWidget->SetProduceWindowName(FText::FromString(TEXT("제작 - 작업대")));
					// 작업대 생산 아이템 추가
					PlaceableProduceWidget->CreateBuildProduceItemSlot(1, 16);
					PlaceableProduceWidget->CreateToolProduceItemSlot(1, 2);
					PlaceableProduceWidget->CreateWeaponProduceItemSlot(3, 4);
					PlaceableProduceWidget->SetButtonVisivility(ESlateVisibility::Visible, ESlateVisibility::Visible, ESlateVisibility::Visible, ESlateVisibility::Collapsed);
					break;
				case EPlaceableStructureType::Furnace:
					PlaceableProduceWidget->SetProduceWindowName(FText::FromString(TEXT("제작 - 화로")));
					// 화로 생산 아이템 추가
					PlaceableProduceWidget->SetButtonVisivility(ESlateVisibility::Collapsed, ESlateVisibility::Collapsed, ESlateVisibility::Collapsed, ESlateVisibility::Visible);
					PlaceableProduceWidget->SetProducePanelSwitcherIndex(3);
					PlaceableProduceWidget->CreateHarvestProduceItemSlot(5, 5);

					IgniteButtonNormalBrush = PlaceableProduceWidget->GetIgniteButton()->WidgetStyle.Normal;
					IgniteButtonPressedBrush = PlaceableProduceWidget->GetIgniteButton()->WidgetStyle.Pressed;
					break;
				case EPlaceableStructureType::CampFire:
					PlaceableProduceWidget->SetProduceWindowName(FText::FromString(TEXT("제작 - 모닥불")));
					// 모닥불 생산 아이템 추가
					PlaceableProduceWidget->SetButtonVisivility(ESlateVisibility::Collapsed, ESlateVisibility::Collapsed, ESlateVisibility::Collapsed, ESlateVisibility::Visible);
					PlaceableProduceWidget->SetProducePanelSwitcherIndex(4);
					PlaceableProduceWidget->CreateConsumableProduceItemSlot(28, 28);

					IgniteButtonNormalBrush = PlaceableProduceWidget->GetIgniteButton()->WidgetStyle.Normal;
					IgniteButtonPressedBrush = PlaceableProduceWidget->GetIgniteButton()->WidgetStyle.Pressed;
					break;
				default:
					break;
				}

			}
		}
		break;
	}

	if (this->HasAuthority())
	{
		CDebug::Print("HasAuthority", FColor::Green);
	}
	else
	{
		CDebug::Print("No HasAuthority", FColor::Red);
	}
}

void ACStructure_Placeable::OpenActorInventory(const ACSurvivor* Survivor, class AActor* Actor)
{
	if (Survivor)
	{
		switch (WidgetCaller)
		{
		case EWidgetCall::Placeable:
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
							mainHUD->SetWidgetVisibility(EWidgetCall::Placeable, ActorInventoryWidget, ActorProduceWidget, Actor);
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

void ACStructure_Placeable::CheckHeight()
{
	// 높이 체크
	FHitResult floorHitResult;
	float startLocationX = this->GetActorLocation().X;
	float startLocationY = this->GetActorLocation().Y;
	float startLocationZ = GetOwner()->GetActorLocation().Z;
	FVector startLocation = FVector(startLocationX, startLocationY, startLocationZ);
	FVector endLocation = startLocation - FVector(0, 0, 300);
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypeQuery;
	objectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));
	TArray<AActor*> ignores;
	FCollisionObjectQueryParams objectQueryParams;
	objectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);
	FCollisionQueryParams collisionParams;
	bLandScapeHit = UKismetSystemLibrary::LineTraceSingleForObjects
	(
		GetWorld(),
		startLocation,
		endLocation,
		objectTypeQuery,
		false,
		ignores,
		EDrawDebugTrace::ForDuration,
		floorHitResult,
		true,
		FLinearColor::Yellow,
		FLinearColor::Red
	);

	AActor* heightHitActor = floorHitResult.GetActor();
	if (IsValid(heightHitActor))
	{
		if (heightHitActor->IsA(ALandscape::StaticClass()))
		{
			PlaceableHeight = floorHitResult.ImpactPoint.Z + 30.0f;
			bLandScapeHit = true;
		}
		else
		{
			PlaceableHeight = GetOwner()->GetActorLocation().Z;
			bLandScapeHit = false;
		}
	}
	else
	{
		PlaceableHeight = GetOwner()->GetActorLocation().Z;
		bLandScapeHit = false;
	}
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
	ETraceTypeQuery centerBoxTraceTypeQuery = ETraceTypeQuery::TraceTypeQuery3;
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

void ACStructure_Placeable::DoBuildTypeInteract()
{
	switch (ItemReference->BuildData.InteractableBuildType)
	{
	case EInteractableBuildType::Bed:
	{
		if (this->HasAuthority())
			CDebug::Print("Server Bed Interaction Called");
		else
			CDebug::Print("Client Bed Interaction Called");
	}
	}
}

void ACStructure_Placeable::PerformAddItem(FName InID, int32 InQuantity, FItemNumericData InNumericData, EItemType InItemType, FItemStats InItemStats)
{
	//FItemInformation addedItemInfo;
	//addedItemInfo.ItemID = InID;
	//addedItemInfo.Quantity = InQuantity;
	//addedItemInfo.NumericData = InNumericData;
	//addedItemInfo.ItemType = InItemType;
	//CDebug::Print("PerformAddItem Durability : ", InItemStats.RemainDurability);
	//addedItemInfo.ItemStats = InItemStats;

	//// 스택이 가능한 아이템인지 검사
	//if (addedItemInfo.NumericData.bIsStackable)
	//{
	//	// 최대 스택이 아니면서 ID가 같은 인덱스를 반환, 못 찾을 경우 -1 반환
	//	int32 resultIndex = GetIndexOfNonFullStackByID(addedItemInfo);
	//	if (resultIndex == -1)
	//		ItemInfoArray.Add(addedItemInfo);
	//	else
	//	{
	//		// 더했을 때 최대 스택을 초과하는지 검사
	//		if (CheckMaxStack(addedItemInfo, resultIndex))
	//		{
	//			int32 addQuantity = ItemInfoArray[resultIndex].NumericData.MaxStackSize - ItemInfoArray[resultIndex].Quantity;
	//			ItemInfoArray[resultIndex].Quantity += addQuantity;

	//			// 최대 스택만큼 더하고 남은 양만큼 다시 PerformAddItem 호출
	//			PerformAddItem(addedItemInfo.ItemID, addedItemInfo.Quantity - addQuantity, addedItemInfo.NumericData, addedItemInfo.ItemType, addedItemInfo.ItemStats);
	//		}
	//		else
	//			ItemInfoArray[resultIndex].Quantity += addedItemInfo.Quantity;
	//	}
	//}
	//else
	//	ItemInfoArray.Add(addedItemInfo);

	//// TArray는 리플리케이트 되더라도 특정 인덱스의 내용물만 바꾸는 것으로는 리플리케이트 되지 않음
	//// 새로 초기화 해주거나 행렬 원소 갯수가 변경되어야 리플리케이트가 적용됨
	//SharedItemInfoArray = ItemInfoArray;
	//// 클라이언트에서 AddItemInfoToWidget() 호출하기위한 OnRep_WidgetRefreshTrigger 트리거
	//WidgetRefreshTrigger++;

	//AddItemInfoToWidget();

	FItemInformation addedItemInfo;
	addedItemInfo.ItemID = InID;
	addedItemInfo.Quantity = InQuantity;
	addedItemInfo.NumericData = InNumericData;
	addedItemInfo.ItemType = InItemType;
	CDebug::Print("PerformAddItem Durability : ", InItemStats.RemainDurability);
	addedItemInfo.ItemStats = InItemStats;

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
				PerformAddItem(addedItemInfo.ItemID, addedItemInfo.Quantity - addQuantity, addedItemInfo.NumericData, addedItemInfo.ItemType, addedItemInfo.ItemStats);
			}
			else
				ItemInfoArray[resultIndex].Quantity += addedItemInfo.Quantity;
		}
	}
	else
		ItemInfoArray.Add(addedItemInfo);

	AddItemInfoToWidget();
}

void ACStructure_Placeable::BroadcastAddItem_Implementation(FName InID, int32 InQuantity, FItemNumericData InNumericData, EItemType InItemType, FItemStats InItemStats)
{
	PerformAddItem(InID, InQuantity, InNumericData, InItemType, InItemStats);
}

void ACStructure_Placeable::PerformRemoveItem(int32 idxRemove)
{
	//ItemInfoArray.RemoveAt(idxRemove);
	//SharedItemInfoArray = ItemInfoArray;
	//AddItemInfoToWidget();
	//WidgetRefreshTrigger++;

	ItemInfoArray.RemoveAt(idxRemove);
	AddItemInfoToWidget();
}

void ACStructure_Placeable::BroadcastRemoveItem_Implementation(int32 idxRemove)
{
	PerformRemoveItem(idxRemove);
}

void ACStructure_Placeable::PerformRemoveAmountOfItem(int32 idxRemove, int32 AmountToRemove)
{
	//ItemInfoArray[idxRemove].Quantity -= AmountToRemove;
	//SharedItemInfoArray = ItemInfoArray;
	//AddItemInfoToWidget();
	//WidgetRefreshTrigger++;

	ItemInfoArray[idxRemove].Quantity -= AmountToRemove;
	AddItemInfoToWidget();
}

void ACStructure_Placeable::BroadcastRemoveAmountOfItem_Implementation(int32 idxRemove, int32 AmountToRemove)
{
	PerformRemoveAmountOfItem(idxRemove, AmountToRemove);
}

void ACStructure_Placeable::PerformSwapItem(int32 idxBase, int32 idxDrag)
{
	//ItemInfoArray.Swap(idxBase, idxDrag);
	//SharedItemInfoArray = ItemInfoArray;
	//AddItemInfoToWidget();
	//WidgetRefreshTrigger++;

	ItemInfoArray.Swap(idxBase, idxDrag);
	AddItemInfoToWidget();
}

void ACStructure_Placeable::BroadcastSwapItem_Implementation(int32 idxBase, int32 idxDrag)
{
	PerformSwapItem(idxBase, idxDrag);
}

void ACStructure_Placeable::PerformCombineItem(int32 idxBase, int32 idxDrag)
{
	//int32 AmountLeftToDistribute = ItemInfoArray[idxDrag].Quantity;
	//const int32 AmountToMakeFullStack = ItemInfoArray[idxBase].NumericData.MaxStackSize - ItemInfoArray[idxBase].Quantity; //남은 수납가능 재고량 계산 
	//if (AmountToMakeFullStack >= AmountLeftToDistribute) //재고량이 넘치면 다 넣고 드래그 아이템 삭제
	//{
	//	ItemInfoArray[idxBase].Quantity += AmountLeftToDistribute;
	//	ItemInfoArray.RemoveAt(idxDrag);

	//}
	//else if (AmountToMakeFullStack < AmountLeftToDistribute) // 다 넣을수 없으면 일부만 넣기 
	//{
	//	//기존 인벤 아이템에 add
	//	ItemInfoArray[idxBase].Quantity += AmountToMakeFullStack;

	//	//넣은 개수만큼 기존의 빼오는 아이템 재고에 Subtract 
	//	AmountLeftToDistribute -= AmountToMakeFullStack;
	//	ItemInfoArray[idxDrag].Quantity = AmountLeftToDistribute;
	//}
	//SharedItemInfoArray = ItemInfoArray;
	//AddItemInfoToWidget();
	//WidgetRefreshTrigger++;

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

	AddItemInfoToWidget();
}

void ACStructure_Placeable::BroadcastCombineItem_Implementation(int32 idxBase, int32 idxDrag)
{
	PerformCombineItem(idxBase, idxDrag);
}

void ACStructure_Placeable::PerformSplitItem(int32 ItemIdx, int32 AmountToSplit)
{
	//FItemInformation addedItemInfo;
	//addedItemInfo.ItemID = ItemInfoArray[ItemIdx].ItemID;
	//addedItemInfo.Quantity = AmountToSplit;
	//addedItemInfo.NumericData = ItemInfoArray[ItemIdx].NumericData;
	//addedItemInfo.ItemType = ItemInfoArray[ItemIdx].ItemType;

	//// 스택이 가능한 아이템인지 검사
	//if (addedItemInfo.NumericData.bIsStackable)
	//{

	//	ItemInfoArray[ItemIdx].Quantity -= AmountToSplit;
	//	ItemInfoArray.Add(addedItemInfo);
	//	SharedItemInfoArray = ItemInfoArray;
	//	AddItemInfoToWidget();
	//	WidgetRefreshTrigger++;

	//}

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
		AddItemInfoToWidget();
	}
}

void ACStructure_Placeable::BroadcastSplitItem_Implementation(int32 ItemIdx, int32 AmountToSplit)
{
	PerformSplitItem(ItemIdx, AmountToSplit);
}

void ACStructure_Placeable::AddItemInfoToWidget()
{
	// 추가하기 전 UCItemBase 배열 초기화
	ActorInventoryContents.Empty();

	//// 리플리케이트된 FItemData의 배열 SharedItemInfoArray를 기반으로 UCItemBase 생성
	//for (int32 tempIndex = 0; tempIndex < SharedItemInfoArray.Num(); tempIndex++)
	//{
	//	FName tempID = SharedItemInfoArray[tempIndex].ItemID;
	//	FItemData* itemData = ItemDataTable->FindRow<FItemData>(tempID, TEXT(""));
	//	if (itemData)
	//	{
	//		UCItemBase* ItemCopy = NewObject<UCItemBase>(StaticClass());
	//		ItemCopy->ID = tempID;
	//		ItemCopy->Quantity = SharedItemInfoArray[tempIndex].Quantity;
	//		ItemCopy->ItemType = itemData->ItemType;
	//		ItemCopy->TextData = itemData->TextData;
	//		ItemCopy->ItemStats = SharedItemInfoArray[tempIndex].ItemStats;
	//		ItemCopy->NumericData = itemData->NumericData;
	//		ItemCopy->AssetData = itemData->AssetData;
	//		ItemCopy->bIsCopy = true;

	//		ActorInventoryContents.Add(ItemCopy);
	//	}
	//	else
	//		CDebug::Print("Itemdata is not Valid", FColor::Magenta);
	//}

	// 리플리케이트된 FItemData의 배열 SharedItemInfoArray를 기반으로 UCItemBase 생성
	for (int32 tempIndex = 0; tempIndex < ItemInfoArray.Num(); tempIndex++)
	{
		FName tempID = ItemInfoArray[tempIndex].ItemID;
		FItemData* itemData = ItemDataTable->FindRow<FItemData>(tempID, TEXT(""));
		if (itemData)
		{
			UCItemBase* ItemCopy = NewObject<UCItemBase>(StaticClass());
			ItemCopy->ID = tempID;
			ItemCopy->Quantity = ItemInfoArray[tempIndex].Quantity;
			ItemCopy->ItemType = itemData->ItemType;
			ItemCopy->TextData = itemData->TextData;
			ItemCopy->ItemStats = ItemInfoArray[tempIndex].ItemStats;
			ItemCopy->NumericData = itemData->NumericData;
			ItemCopy->AssetData = itemData->AssetData;
			ItemCopy->bIsCopy = true;

			ActorInventoryContents.Add(ItemCopy);
		}
		else
			CDebug::Print("Itemdata is not Valid", FColor::Magenta);
	}

	//UI 위젯에 아이템 인벤토리 변경사항 업데이트 
	switch (WidgetCaller)
	{
	case EWidgetCall::Placeable:
	{
		UCInventoryPanel_Placeable* placeableInventoryWidget = Cast<UCInventoryPanel_Placeable>(ActorInventoryWidget);
		if (placeableInventoryWidget)
		{
			// Placeable의 클래스에 UCItemBase의 배열 전달
			placeableInventoryWidget->SetWidgetItems(ActorInventoryContents);
			placeableInventoryWidget->RefreshPlaceableInventory();
		}

		UCProduceWidget* placeableProduceWidget = Cast<UCProduceWidget>(ActorProduceWidget);
		if (placeableProduceWidget)
		{
			placeableProduceWidget->RefreshProduceDetail();
		}
	}
	}
}

//void ACStructure_Placeable::OnRep_WidgetRefreshTrigger()
//{
//	AddItemInfoToWidget();
//}

void ACStructure_Placeable::BroadcastAddProduceItemToQueue_Implementation(FName ItemID, class ACStructure_Placeable* InPlaceable)
{
	PlaceableProduceWidget->AddProduceItemToQueue(ItemID);
}

void ACStructure_Placeable::BroadcastRemoveProduceItemFromQueue_Implementation(int32 InIndex)
{
	PlaceableProduceWidget->GetProduceQueue()->RemoveChildAt(InIndex);
	PlaceableProduceWidget->CheckWrapBox(PlaceableProduceWidget->GetProduceQueue());
}

void ACStructure_Placeable::BroadcastIgnite_Implementation()
{
	this->GetPlaceableProduceWidget()->SetIgniteButtonOn();
	PerformIgnite();
}

void ACStructure_Placeable::BroadcastExtinguish_Implementation()
{
	this->GetPlaceableProduceWidget()->SetIgniteButtonOff();
	PerformExtinguish();
}

void ACStructure_Placeable::PerformIgnite()
{
	if (FireParticle && FireSound && IgniteSound && FireSoundAttenuation)
	{
		FVector adjustLocation = FVector::ZeroVector;
		FVector adjustScale = FVector::ZeroVector;

		switch (PlaceableStructureType)
		{
		case EPlaceableStructureType::Furnace:
			adjustLocation = FVector(0.0f, 0.0f, 110.0f);
			adjustScale = FVector(1.0f, 1.0f, 1.0f);
			break;
		case EPlaceableStructureType::CampFire:
			adjustLocation = FVector(0.0f, 0.0f, 40.0f);
			adjustScale = FVector(0.5f, 0.5f, 0.5f);
			break;
		default:
			break;
		}

		SpawnedFireParticle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			FireParticle,
			this->GetActorLocation() + adjustLocation,
			this->GetActorRotation(),
			true
		);
		SpawnedFireParticle->SetWorldScale3D(adjustScale);
	
		UGameplayStatics::SpawnSoundAtLocation(
			GetWorld(),
			IgniteSound,
			this->GetActorLocation() + adjustLocation,
			this->GetActorRotation(),
			1.0f,
			1.0f,
			0.0f,
			FireSoundAttenuation
		);

		SpawnedFireSound = UGameplayStatics::SpawnSoundAtLocation(
			GetWorld(),
			FireSound,
			this->GetActorLocation() + adjustLocation,
			this->GetActorRotation(),
			0.25f,
			1.0f,
			0.0f,
			FireSoundAttenuation
		);

		bIsIgnited = true;

		GetWorld()->GetTimerManager().SetTimer(IgniteTimerHandle, this, &ACStructure_Placeable::CheckWoodResource, 3.0f, true);
	}
}

void ACStructure_Placeable::PerformExtinguish()
{
	if (SpawnedFireParticle && SpawnedFireSound && ExtinguishSound)
	{
		SpawnedFireParticle->DestroyComponent();

		UGameplayStatics::SpawnSoundAtLocation(
			GetWorld(),
			ExtinguishSound,
			this->GetActorLocation() + FVector(0.0f, 0.0f, 110.0f),
			this->GetActorRotation(),
			1.0f,
			1.0f,
			0.0f,
			FireSoundAttenuation
		);

		SpawnedFireSound->DestroyComponent();
		bIsIgnited = false;
	}

	if (GetWorld()->GetTimerManager().IsTimerActive(IgniteTimerHandle))
		GetWorld()->GetTimerManager().ClearTimer(IgniteTimerHandle);
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

void ACStructure_Placeable::CheckWoodResource()
{
	CDebug::Print("CheckWoodResource Called");
	
	if (!PlaceableProduceWidget)
		return;

	if (this->PlaceableProduceWidget->GetOwningPlayer()->HasAuthority())
	{
		if (!this->PlaceableProduceWidget->CheckWoodResourceUsed())
		{
			BroadcastExtinguish();
		}
	}
}

//타입 , 수량 순으로 정렬 
void ACStructure_Placeable::PerformSortInfoWidget()
{
	//if (ItemInfoArray.Num() == 0) return;


	////병합 정렬 사용 
	//MergeSort(ItemInfoArray, 0, ItemInfoArray.Num() - 1);
	//SharedItemInfoArray = ItemInfoArray;
	//AddItemInfoToWidget();
	//WidgetRefreshTrigger++;

	if (ItemInfoArray.Num() == 0) return;

	//병합 정렬 사용 
	MergeSort(ItemInfoArray, 0, ItemInfoArray.Num() - 1);
	AddItemInfoToWidget();
}

void ACStructure_Placeable::BroadcastSortInfoWidget_Implementation()
{
	PerformSortInfoWidget();
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
			(LeftArray[i].ItemType == RightArray[j].ItemType && LeftIDNumber < RightIDNumber)
			|| (LeftArray[i].ItemType == RightArray[j].ItemType && LeftIDNumber == RightIDNumber && LeftArray[i].Quantity > RightArray[j].Quantity)) //아이템 타입 Enum 내림차순 , 아이템 ID 오름차순,Quantity 내림차순으로 정렬 
		{
			Array[k] = LeftArray[i];
			i++;
		}
		else
		{
			Array[k] = RightArray[j];
			j++;
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

//void ACStructure_Placeable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	//DOREPLIFETIME(ACStructure_Placeable, WidgetRefreshTrigger);
//	//DOREPLIFETIME(ACStructure_Placeable, SharedItemInfoArray);
//}