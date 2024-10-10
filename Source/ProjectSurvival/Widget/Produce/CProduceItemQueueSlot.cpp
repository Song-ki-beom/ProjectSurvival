#include "Widget/Produce/CProduceItemQueueSlot.h"
#include "Widget/Produce/CProduceWidget.h"
#include "Character/CSurvivor.h"
#include "Character/CSurvivorController.h"
#include "ActorComponents/CInventoryComponent.h"
#include "Widget/Inventory/CItemBase.h"
#include "Widget/Inventory/CInventoryPanel_Placeable.h"
#include "Widget/Build/CBuildWidget.h"
#include "Build/CStructure_Placeable.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "Components/ProgressBar.h"
#include "Utility/CDebug.h"

bool UCProduceItemQueueSlot::Initialize()
{
	bool Sucess = Super::Initialize();

	if (!Sucess)
		return false;

	if (!IsValid(ProduceCancleButton)) { CDebug::Print("ProduceCancleButton is invalid"); return false; }
	ProduceCancleButton->OnClicked.AddDynamic(this, &UCProduceItemQueueSlot::CancleProduce);

	return true;
}

void UCProduceItemQueueSlot::SetProduceItemID(FName InID)
{
	ProduceItemID = InID;
}

void UCProduceItemQueueSlot::SetProduceQueueSlotIcon(UTexture2D* InTexture2D)
{
	ProduceItemQueueSlotIcon->SetBrushFromTexture(InTexture2D);
}

void UCProduceItemQueueSlot::SetProduceTimeText(FText InText)
{
	ProduceTimeText->SetText(InText);
}

void UCProduceItemQueueSlot::SetProduceItemName(FText InText)
{
	ProduceItemName = InText;
}

void UCProduceItemQueueSlot::SetProduceWidgetData(FProduceWidgetData InProduceWidgetData)
{
	ProduceWidgetData = InProduceWidgetData;
}

void UCProduceItemQueueSlot::InitProduce()
{
	//CDebug::Print("InitProduce Called", FColor::White);
	bIsInitialized = true;

	TotalProduceTime = FCString::Atof(*ProduceTimeText->GetText().ToString());
	RemainProduceTime = TotalProduceTime;

	class UDataTable* itemDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, TEXT("DataTable'/Game/PirateIsland/Include/Datas/Widget/Inventory/DT_Items.DT_Items'")));
	FItemData* itemData = itemDataTable->FindRow<FItemData>(ProduceItemID, TEXT(""));
	if (itemData)
	{
		ProduceTargetItem = NewObject<UCItemBase>(StaticClass());
		ProduceTargetItem->ID = ProduceItemID;
		ProduceTargetItem->Quantity = 1;
		ProduceTargetItem->ItemType = itemData->ItemType;
		ProduceTargetItem->TextData = itemData->TextData;
		ProduceTargetItem->ItemStats = itemData->ItemStats;
		ProduceTargetItem->NumericData = itemData->NumericData;
		ProduceTargetItem->AssetData = itemData->AssetData;
		ProduceTargetItem->ProduceData = itemData->ProduceData;
		ProduceTargetItem->BuildData = itemData->BuildData;
		ProduceTargetItem->HuntData = itemData->HuntData;
		ProduceTargetItem->bIsCopy = true;
	}

	UCProduceWidget* produceWidget = Cast<UCProduceWidget>(this->GetParent()->GetTypedOuter<UUserWidget>());
	if (produceWidget)
	{
		if (produceWidget->GetOwnerActor())
		{
			bIsPlaceableOwner = true;

			// 화력을 사용하는 Placeable인지 체크
			EPlaceableStructureType placeableStructureType = produceWidget->GetOwnerActor()->GetPlaceableStructureType();
			if (placeableStructureType == EPlaceableStructureType::Furnace || placeableStructureType == EPlaceableStructureType::CampFire)
				IgniteUsingPlaceable = produceWidget->GetOwnerActor();
		}
	}

	Survivor = Cast<ACSurvivor>(this->GetOwningPlayerPawn());

	if (Survivor && ProduceTargetItem)
	{
		GetWorld()->GetTimerManager().SetTimer(ProgressTimerHandle, this, &UCProduceItemQueueSlot::SetProduceProgress, 0.1f, true, 0.0f);
		GetWorld()->GetTimerManager().PauseTimer(ProgressTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(PauseProgressTimerHandle, this, &UCProduceItemQueueSlot::PauseProduceProgress, 0.1f, true, 0.0f);
		GetWorld()->GetTimerManager().PauseTimer(PauseProgressTimerHandle);
	}

	ProduceWidget = Cast<UCProduceWidget>(this->GetParent()->GetTypedOuter<UUserWidget>());
}

void UCProduceItemQueueSlot::StartProduce()
{
	bIsProducing = true;

	if (ProduceWidget)
	{
		FText produceItemNameText = FText::Format(FText::FromString(TEXT("생산 중: {0}")), ProduceItemName);
		ProduceWidget->SetProducingItemText(produceItemNameText);
	}
	else
		CDebug::Print("produceWidget is not valid", FColor::Magenta);

	GetWorld()->GetTimerManager().UnPauseTimer(ProgressTimerHandle);
}

void UCProduceItemQueueSlot::SetProduceProgress()
{
	if (bIsPlaceableOwner)
	{
		if (IgniteUsingPlaceable)
		{
			if (!IgniteUsingPlaceable->GetIgniteState())
			{
				GetWorld()->GetTimerManager().PauseTimer(ProgressTimerHandle);
				GetWorld()->GetTimerManager().UnPauseTimer(PauseProgressTimerHandle);

				RemainProduceTime += 0.1f;
				FText produceTimeText = FText::Format(FText::FromString(TEXT("{0}초")), FText::AsNumber(RemainProduceTime, &FNumberFormattingOptions().SetMaximumFractionalDigits(1)));
				ProduceTimeText->SetText(produceTimeText);

				float progress = (TotalProduceTime - RemainProduceTime) / TotalProduceTime;
				ProduceProgressBar->SetPercent(progress);

				FText produceItemNameText = FText::Format(FText::FromString(TEXT("생산 중단됨: {0} - 화력 부족")), ProduceItemName);
				ProduceWidget->SetProducingItemText(produceItemNameText, FLinearColor::Yellow);
			}
		}
	}
	else
	{
		int32 totalWeight = Survivor->GetInventoryComponent()->GetWeightCapacity();
		int32 inventoryWeight = Survivor->GetInventoryComponent()->GetInventoryTotalWeight();

		if (inventoryWeight + ProduceTargetItem->NumericData.Weight > totalWeight)
		{
			GetWorld()->GetTimerManager().PauseTimer(ProgressTimerHandle);
			GetWorld()->GetTimerManager().UnPauseTimer(PauseProgressTimerHandle);


			RemainProduceTime += 0.1f;
			FText produceTimeText = FText::Format(FText::FromString(TEXT("{0}초")), FText::AsNumber(RemainProduceTime, &FNumberFormattingOptions().SetMaximumFractionalDigits(1)));
			ProduceTimeText->SetText(produceTimeText);

			float progress = (TotalProduceTime - RemainProduceTime) / TotalProduceTime;
			ProduceProgressBar->SetPercent(progress);

			FText produceItemNameText = FText::Format(FText::FromString(TEXT("생산 중단됨: {0} - 무게 초과")), ProduceItemName);
			ProduceWidget->SetProducingItemText(produceItemNameText, FLinearColor::Yellow);
		}
	}

	RemainProduceTime -= 0.1f;
	FText produceTimeText = FText::Format(FText::FromString(TEXT("{0}초")), FText::AsNumber(RemainProduceTime, &FNumberFormattingOptions().SetMaximumFractionalDigits(1)));
	ProduceTimeText->SetText(produceTimeText);

	float progress = (TotalProduceTime - RemainProduceTime) / TotalProduceTime;
	ProduceProgressBar->SetPercent(progress);

	if (RemainProduceTime <= 0.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(ProgressTimerHandle);
		if (this->GetParent())
		{
			UCProduceWidget* produceWidget = Cast<UCProduceWidget>(this->GetParent()->GetTypedOuter<UUserWidget>());
			if (produceWidget)
			{
				switch (produceWidget->GetWidgetCall())
				{
				case EWidgetCall::Survivor:
				{
					Survivor->GetInventoryComponent()->HandleAddItem(ProduceTargetItem);
					ACSurvivorController* survivorController = Cast<ACSurvivorController>(Survivor->GetController());
					if (survivorController)
					{
						if (survivorController->GetBuildWidget())
							survivorController->GetBuildWidget()->RefreshBuildWidgetQuantity(ProduceItemID);
						else
							CDebug::Print("survivorController->GetBuildWidget() is not Valid", FColor::Red);
					}
					else
						CDebug::Print("survivorController is not Valid", FColor::Red);
					break;
				}
				case EWidgetCall::Placeable:
					if (produceWidget->GetOwnerActor()->HasAuthority())
					{
						produceWidget->GetOwnerActor()->BroadcastAddItem(ProduceTargetItem->ID, 1, ProduceTargetItem->NumericData, ProduceTargetItem->ItemType, ProduceTargetItem->ItemStats, ProduceTargetItem->HuntData.WeaponType);
					}
					break;
				}
			}
			EndProduce();
		}
	}
}

void UCProduceItemQueueSlot::PauseProduceProgress()
{
	if (bIsPlaceableOwner)
	{
		if (IgniteUsingPlaceable)
		{
			if (IgniteUsingPlaceable->GetIgniteState())
			{
				GetWorld()->GetTimerManager().PauseTimer(PauseProgressTimerHandle);
				GetWorld()->GetTimerManager().UnPauseTimer(ProgressTimerHandle);
				
				FText produceItemNameText = FText::Format(FText::FromString(TEXT("생산 중: {0}")), ProduceItemName);
				ProduceWidget->SetProducingItemText(produceItemNameText);
			}
		}
	}
	else
	{
		int32 totalWeight = Survivor->GetInventoryComponent()->GetWeightCapacity();
		int32 inventoryWeight = Survivor->GetInventoryComponent()->GetInventoryTotalWeight();

		if (inventoryWeight < totalWeight)
		{
			GetWorld()->GetTimerManager().PauseTimer(PauseProgressTimerHandle);
			GetWorld()->GetTimerManager().UnPauseTimer(ProgressTimerHandle);

			FText produceItemNameText = FText::Format(FText::FromString(TEXT("생산 중: {0}")), ProduceItemName);
			ProduceWidget->SetProducingItemText(produceItemNameText);
		}
	}
}

void UCProduceItemQueueSlot::EndProduce()
{
	class UWrapBox* wrapBox = Cast<UWrapBox>(this->GetParent());
	if (wrapBox)
	{
		wrapBox->RemoveChild(this);

		UCProduceWidget* produceWidget = Cast<UCProduceWidget>(wrapBox->GetTypedOuter<UUserWidget>());
		if (produceWidget)
			produceWidget->CheckWrapBox(wrapBox);
	}
}

//void UCProduceItemQueueSlot::CheckWrapBox(class UWrapBox* InWrapBox)
//{
//	if (InWrapBox)
//	{
//		const TArray<UWidget*>& wrapBoxChildren = InWrapBox->GetAllChildren();
//
//		for (int index = 0; index < wrapBoxChildren.Num(); index++)
//		{
//			UCProduceItemQueueSlot* produceItemQueueSlot = Cast<UCProduceItemQueueSlot>(wrapBoxChildren[index]);
//			if (produceItemQueueSlot)
//			{
//				if (index == 0)
//				{
//					if (produceItemQueueSlot->bIsInitialized)
//					{
//						if (!(produceItemQueueSlot->bIsProducing))
//							produceItemQueueSlot->StartProduce();
//					}
//					else
//					{
//						produceItemQueueSlot->InitProduce();
//						produceItemQueueSlot->StartProduce();
//					}
//
//				}
//				else
//				{
//					if (!produceItemQueueSlot->bIsInitialized)
//						produceItemQueueSlot->InitProduce();
//				}
//
//			}
//		}
//
//		if (!(wrapBoxChildren.Num() > 0))
//		{
//			if (ProduceWidget)
//			{
//				FText produceItemNameText = FText::GetEmpty();
//				ProduceWidget->SetProducingItemText(produceItemNameText);
//			}
//			else
//				CDebug::Print("produceWidget is not valid", FColor::Red);
//
//			return;
//		}
//	}
//	else
//		CDebug::Print("WrapBox is not Valid", FColor::White);
//}

void UCProduceItemQueueSlot::RemoveProduceItemQueueSlotWidget()
{
	UCProduceWidget* produceWidget = Cast<UCProduceWidget>(this->GetParent()->GetTypedOuter<UUserWidget>());
	if (produceWidget)
	{

		if (bIsPlaceableOwner)
		{
			//UCProduceWidget* produceWidget = Cast<UCProduceWidget>(this->GetParent()->GetTypedOuter<UUserWidget>());
			//if (produceWidget)
			{
				class UWrapBox* wrapBox = Cast<UWrapBox>(this->GetParent());
				if (wrapBox)
				{
					for (int32 i = 0; i < wrapBox->GetAllChildren().Num(); i++)
					{
						UWidget* itemSlot = wrapBox->GetChildAt(i);

						if (itemSlot == this)
						{
							if (produceWidget->GetOwnerActor())
							{
								if (this->GetOwningPlayer()->HasAuthority())
								{
									produceWidget->GetOwnerActor()->BroadcastRemoveProduceItemFromQueue(i);
								}
								else
								{
									ACSurvivorController* survivorController = Cast<ACSurvivorController>(this->GetOwningPlayer());
									if (survivorController)
									{
										survivorController->RequestRemoveProduceItemFromQueue(produceWidget->GetOwnerActor(), i);
									}
								}
							}
							break;
						}
					}
				}
			}
		}
		else
		{
			class UWrapBox* wrapBox = Cast<UWrapBox>(this->GetParent());
			if (wrapBox)
			{
				CDebug::Print("wrapBox is valid");
				wrapBox->RemoveChild(this);
			}
			else
				CDebug::Print("wrapBox is not valid");

			produceWidget->CheckWrapBox(wrapBox);
		}
	}
}

void UCProduceItemQueueSlot::CancleProduce()
{
	GetWorld()->GetTimerManager().ClearTimer(ProgressTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(PauseProgressTimerHandle);

	class UDataTable* itemDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, TEXT("DataTable'/Game/PirateIsland/Include/Datas/Widget/Inventory/DT_Items.DT_Items'")));
	
	FName resourceID_1 = ProduceWidgetData.ProduceResource_1.ResourceID;
	FName resourceID_2 = ProduceWidgetData.ProduceResource_2.ResourceID;
	FName resourceID_3 = ProduceWidgetData.ProduceResource_3.ResourceID;
	FName resourceID_4 = ProduceWidgetData.ProduceResource_4.ResourceID;
	FName resourceID_5 = ProduceWidgetData.ProduceResource_5.ResourceID;

	CDebug::Print("resourceID_1", resourceID_1);

	if (resourceID_1 != NAME_None)
	{
		FItemData* cancleResourceData = itemDataTable->FindRow<FItemData>(resourceID_1, TEXT(""));
		if (cancleResourceData)
		{
			class UCItemBase* cancleItem = NewObject<UCItemBase>(StaticClass());
			if (cancleItem)
			{
				cancleItem = NewObject<UCItemBase>(StaticClass());
				cancleItem->ID = ProduceWidgetData.ProduceResource_1.ResourceID;
				cancleItem->Quantity = ProduceWidgetData.ProduceResource_1.ProduceResourceDemand;
				cancleItem->ItemType = cancleResourceData->ItemType;
				cancleItem->TextData = cancleResourceData->TextData;
				cancleItem->ItemStats = cancleResourceData->ItemStats;
				cancleItem->NumericData = cancleResourceData->NumericData;
				cancleItem->AssetData = cancleResourceData->AssetData;
				cancleItem->bIsCopy = true;

				if (bIsPlaceableOwner)
				{
					UCProduceWidget* produceWidget = Cast<UCProduceWidget>(this->GetParent()->GetTypedOuter<UUserWidget>());
					if (produceWidget)
					{
						if (produceWidget->GetOwnerActor())
							produceWidget->GetOwnerActor()->GetPlaceableInventoryWidget()->AddItem(cancleItem, cancleItem->Quantity, produceWidget->GetOwnerActor());
					}
				}
				else
					Survivor->GetInventoryComponent()->HandleAddItem(cancleItem);
			}
		}
	}

	if (resourceID_2 != NAME_None)
	{
		FItemData* cancleResourceData = itemDataTable->FindRow<FItemData>(resourceID_2, TEXT(""));
		if (cancleResourceData)
		{
			class UCItemBase* cancleItem = NewObject<UCItemBase>(StaticClass());
			if (cancleItem)
			{
				cancleItem = NewObject<UCItemBase>(StaticClass());
				cancleItem->ID = ProduceWidgetData.ProduceResource_2.ResourceID;
				cancleItem->Quantity = ProduceWidgetData.ProduceResource_2.ProduceResourceDemand;
				cancleItem->ItemType = cancleResourceData->ItemType;
				cancleItem->TextData = cancleResourceData->TextData;
				cancleItem->ItemStats = cancleResourceData->ItemStats;
				cancleItem->NumericData = cancleResourceData->NumericData;
				cancleItem->AssetData = cancleResourceData->AssetData;
				cancleItem->bIsCopy = true;

				if (bIsPlaceableOwner)
				{
					UCProduceWidget* produceWidget = Cast<UCProduceWidget>(this->GetParent()->GetTypedOuter<UUserWidget>());
					if (produceWidget)
					{
						if (produceWidget->GetOwnerActor())
							produceWidget->GetOwnerActor()->GetPlaceableInventoryWidget()->AddItem(cancleItem, cancleItem->Quantity, produceWidget->GetOwnerActor());
					}
				}
				else
					Survivor->GetInventoryComponent()->HandleAddItem(cancleItem);
			}
		}
	}

	if (resourceID_3 != NAME_None)
	{
		FItemData* cancleResourceData = itemDataTable->FindRow<FItemData>(resourceID_3, TEXT(""));
		if (cancleResourceData)
		{
			class UCItemBase* cancleItem = NewObject<UCItemBase>(StaticClass());
			if (cancleItem)
			{
				cancleItem = NewObject<UCItemBase>(StaticClass());
				cancleItem->ID = ProduceWidgetData.ProduceResource_3.ResourceID;
				cancleItem->Quantity = ProduceWidgetData.ProduceResource_3.ProduceResourceDemand;
				cancleItem->ItemType = cancleResourceData->ItemType;
				cancleItem->TextData = cancleResourceData->TextData;
				cancleItem->ItemStats = cancleResourceData->ItemStats;
				cancleItem->NumericData = cancleResourceData->NumericData;
				cancleItem->AssetData = cancleResourceData->AssetData;
				cancleItem->bIsCopy = true;

				if (bIsPlaceableOwner)
				{
					UCProduceWidget* produceWidget = Cast<UCProduceWidget>(this->GetParent()->GetTypedOuter<UUserWidget>());
					if (produceWidget)
					{
						if (produceWidget->GetOwnerActor())
							produceWidget->GetOwnerActor()->GetPlaceableInventoryWidget()->AddItem(cancleItem, cancleItem->Quantity, produceWidget->GetOwnerActor());
					}
				}
				else
					Survivor->GetInventoryComponent()->HandleAddItem(cancleItem);
			}
		}
	}

	if (resourceID_4 != NAME_None)
	{
		FItemData* cancleResourceData = itemDataTable->FindRow<FItemData>(resourceID_4, TEXT(""));
		if (cancleResourceData)
		{
			class UCItemBase* cancleItem = NewObject<UCItemBase>(StaticClass());
			if (cancleItem)
			{
				cancleItem = NewObject<UCItemBase>(StaticClass());
				cancleItem->ID = ProduceWidgetData.ProduceResource_4.ResourceID;
				cancleItem->Quantity = ProduceWidgetData.ProduceResource_4.ProduceResourceDemand;
				cancleItem->ItemType = cancleResourceData->ItemType;
				cancleItem->TextData = cancleResourceData->TextData;
				cancleItem->ItemStats = cancleResourceData->ItemStats;
				cancleItem->NumericData = cancleResourceData->NumericData;
				cancleItem->AssetData = cancleResourceData->AssetData;
				cancleItem->bIsCopy = true;

				if (bIsPlaceableOwner)
				{
					UCProduceWidget* produceWidget = Cast<UCProduceWidget>(this->GetParent()->GetTypedOuter<UUserWidget>());
					if (produceWidget)
					{
						if (produceWidget->GetOwnerActor())
							produceWidget->GetOwnerActor()->GetPlaceableInventoryWidget()->AddItem(cancleItem, cancleItem->Quantity, produceWidget->GetOwnerActor());
					}
				}
				else
					Survivor->GetInventoryComponent()->HandleAddItem(cancleItem);
			}
		}
	}

	if (resourceID_5 != NAME_None)
	{
		FItemData* cancleResourceData = itemDataTable->FindRow<FItemData>(resourceID_5, TEXT(""));
		if (cancleResourceData)
		{
			class UCItemBase* cancleItem = NewObject<UCItemBase>(StaticClass());
			if (cancleItem)
			{
				cancleItem = NewObject<UCItemBase>(StaticClass());
				cancleItem->ID = ProduceWidgetData.ProduceResource_5.ResourceID;
				cancleItem->Quantity = ProduceWidgetData.ProduceResource_5.ProduceResourceDemand;
				cancleItem->ItemType = cancleResourceData->ItemType;
				cancleItem->TextData = cancleResourceData->TextData;
				cancleItem->ItemStats = cancleResourceData->ItemStats;
				cancleItem->NumericData = cancleResourceData->NumericData;
				cancleItem->AssetData = cancleResourceData->AssetData;
				cancleItem->bIsCopy = true;

				if (bIsPlaceableOwner)
				{
					UCProduceWidget* produceWidget = Cast<UCProduceWidget>(this->GetParent()->GetTypedOuter<UUserWidget>());
					if (produceWidget)
					{
						if (produceWidget->GetOwnerActor())
							produceWidget->GetOwnerActor()->GetPlaceableInventoryWidget()->AddItem(cancleItem, cancleItem->Quantity, produceWidget->GetOwnerActor());
					}
				}
				else
					Survivor->GetInventoryComponent()->HandleAddItem(cancleItem);
			}
		}
	}
	
	RemoveProduceItemQueueSlotWidget();

	if (ProduceWidget)
		ProduceWidget->RefreshProduceDetail();
	else
		CDebug::Print("ProduceWidget is not Valid", FColor::White);
}
