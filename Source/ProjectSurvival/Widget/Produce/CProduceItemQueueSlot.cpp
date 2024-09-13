#include "Widget/Produce/CProduceItemQueueSlot.h"
#include "Widget/Produce/CProduceWidget.h"
#include "Character/CSurvivor.h"
#include "ActorComponents/CInventoryComponent.h"
#include "Widget/Inventory/CItemBase.h"
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
		ProduceTargetItem->bIsCopy = true;
	}
	Survivor = Cast<ACSurvivor>(this->GetOwningPlayerPawn());
	if (Survivor)
		CDebug::Print("Survivor is Valid", FColor::White);

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
	//CDebug::Print("StartProduce Called", FColor::White);

	bIsProducing = true;

	if (ProduceWidget)
	{
		CDebug::Print("ProduceItemName", ProduceItemName, FColor::Silver);
		FText produceItemNameText = FText::Format(FText::FromString(TEXT("생산 중: {0}")), ProduceItemName);
		ProduceWidget->SetProducingItemText(produceItemNameText);
	}
	else
		CDebug::Print("produceWidget is not valid", FColor::Magenta);

	GetWorld()->GetTimerManager().UnPauseTimer(ProgressTimerHandle);
}

void UCProduceItemQueueSlot::SetProduceProgress()
{
	//CDebug::Print("Produce Timer ON", FColor::White);

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

	RemainProduceTime -= 0.1f;
	FText produceTimeText = FText::Format(FText::FromString(TEXT("{0}초")), FText::AsNumber(RemainProduceTime, &FNumberFormattingOptions().SetMaximumFractionalDigits(1)));
	ProduceTimeText->SetText(produceTimeText);

	float progress = (TotalProduceTime - RemainProduceTime) / TotalProduceTime;
	ProduceProgressBar->SetPercent(progress);

	if (RemainProduceTime <= 0.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(ProgressTimerHandle);
		Survivor->GetInventoryComponent()->HandleAddItem(ProduceTargetItem);
		EndProduce();
	}
}

void UCProduceItemQueueSlot::PauseProduceProgress()
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

void UCProduceItemQueueSlot::EndProduce()
{
	class UWrapBox* wrapBox = Cast<UWrapBox>(this->GetParent());
	if (wrapBox)
		wrapBox->RemoveChild(this);

	CheckWrapBox(wrapBox);
}

void UCProduceItemQueueSlot::CheckWrapBox(class UWrapBox* InWrapBox)
{
	if (InWrapBox)
	{
		const TArray<UWidget*>& wrapBoxChildren = InWrapBox->GetAllChildren();

		for (int index = 0; index < wrapBoxChildren.Num(); index++)
		{
			UCProduceItemQueueSlot* produceItemQueueSlot = Cast<UCProduceItemQueueSlot>(wrapBoxChildren[index]);
			if (produceItemQueueSlot)
			{
				if (index == 0)
				{
					if (produceItemQueueSlot->bIsInitialized)
					{
						if (!(produceItemQueueSlot->bIsProducing))
							produceItemQueueSlot->StartProduce();
					}
					else
					{
						produceItemQueueSlot->InitProduce();
						produceItemQueueSlot->StartProduce();
					}
					
				}
				else
				{
					if (!produceItemQueueSlot->bIsInitialized)
						produceItemQueueSlot->InitProduce();
				}

			}
		}

		if (!(wrapBoxChildren.Num() > 0))
		{
			if (ProduceWidget)
			{
				FText produceItemNameText = FText::GetEmpty();
				ProduceWidget->SetProducingItemText(produceItemNameText);
			}
			else
				CDebug::Print("produceWidget is not valid");

			CDebug::Print("Children.Num() <= 0: False", FColor::Red);
			return;
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

				Survivor->GetInventoryComponent()->HandleAddItem(cancleItem);
			}
		}
	}
	
	class UWrapBox* wrapBox = Cast<UWrapBox>(this->GetParent());
	if (wrapBox)
	{
		CDebug::Print("wrapBox is valid");
		wrapBox->RemoveChild(this);
	}
	else
		CDebug::Print("wrapBox is not valid");

	CheckWrapBox(wrapBox);

	if (ProduceWidget)
		ProduceWidget->RefreshProduceDetail();
	else
		CDebug::Print("ProduceWidget is not Valid", FColor::White);
}

void UCProduceItemQueueSlot::GetCancleResource(UCItemBase* InItem)
{

}
