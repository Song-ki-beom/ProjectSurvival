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

void UCProduceItemQueueSlot::StartProduce()
{
	CDebug::Print("StartProduce Called");
	bIsWaiting = false;

	ProduceWidget = Cast<UCProduceWidget>(this->GetParent()->GetTypedOuter<UUserWidget>());
	if (ProduceWidget)
	{
		CDebug::Print("ProduceItemName", ProduceItemName, FColor::Silver);
		FText produceItemNameText = FText::Format(FText::FromString(TEXT("생산 중: {0}")), ProduceItemName);
		ProduceWidget->SetProducingItemText(produceItemNameText);
	}
	else
		CDebug::Print("produceWidget is not valid", FColor::Magenta);

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
	if (Survivor && ProduceTargetItem)
	{
		GetWorld()->GetTimerManager().SetTimer(ProgressTimerHandle, this, &UCProduceItemQueueSlot::SetProduceProgress, 0.1f, true);
		GetWorld()->GetTimerManager().SetTimer(PauseProgressTimerHandle, this, &UCProduceItemQueueSlot::PauseProduceProgress, 0.1f, true, 0.0f);
		GetWorld()->GetTimerManager().PauseTimer(PauseProgressTimerHandle);
	}
}

void UCProduceItemQueueSlot::SetProduceProgress()
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

	RemainProduceTime -= 0.1f;
	FText produceTimeText = FText::Format(FText::FromString(TEXT("{0}초")), FText::AsNumber(RemainProduceTime, &FNumberFormattingOptions().SetMaximumFractionalDigits(1)));
	ProduceTimeText->SetText(produceTimeText);

	float progress = (TotalProduceTime - RemainProduceTime) / TotalProduceTime;
	ProduceProgressBar->SetPercent(progress);

	if (RemainProduceTime <= 0.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(ProgressTimerHandle);
		EndProduce();
		Survivor->GetInventoryComponent()->HandleAddItem(ProduceTargetItem);
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
		const TArray<UWidget*>& children = InWrapBox->GetAllChildren();

		if (children.Num() > 0)
		{
			CDebug::Print("Children.Num() > 0: True", FColor::Green);

			UCProduceItemQueueSlot* firstSlot = Cast<UCProduceItemQueueSlot>(children[0]);

			if (firstSlot)
			{
				CDebug::Print("First slot found", FColor::Green);
				if (firstSlot->bIsWaiting)
				{
					CDebug::Print("FirstSlot->bIsWaiting: True", FColor::Green);
					CDebug::Print("UpdateWrapBox And Start Produce for the first slot", FColor::Blue);
					firstSlot->StartProduce();
				}
				else
				{
					CDebug::Print("FirstSlot->bIsWaiting: False", FColor::Red);
					CDebug::Print("First slot is already producing", FColor::Red);
				}
			}
			else
			{
				CDebug::Print("FirstSlot is not a UCProduceItemQueueSlot", FColor::Red);
			}
		}
		else
		{
			if (ProduceWidget)
			{
				FText produceItemNameText = FText::GetEmpty();
				ProduceWidget->SetProducingItemText(produceItemNameText);
			}
			else
				CDebug::Print("produceWidget is not valid");

			CDebug::Print("Children.Num() <= 0: False", FColor::Red);
		}
	}
}

void UCProduceItemQueueSlot::CancleProduce()
{
	//class UDataTable* itemDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, TEXT("DataTable'/Game/PirateIsland/Include/Datas/Widget/Inventory/DT_Items.DT_Items'")));
	
	//FItemData* cancleResource1 = itemDataTable->FindRow<FItemData>(ProduceTargetItem, TEXT(""));
	//if (itemData)
	//{
	//	ProduceTargetItem = NewObject<UCItemBase>(StaticClass());
	//	ProduceTargetItem->ID = ProduceItemID;
	//	ProduceTargetItem->Quantity = 1;
	//	ProduceTargetItem->ItemType = itemData->ItemType;
	//	ProduceTargetItem->TextData = itemData->TextData;
	//	ProduceTargetItem->ItemStats = itemData->ItemStats;
	//	ProduceTargetItem->NumericData = itemData->NumericData;
	//	ProduceTargetItem->AssetData = itemData->AssetData;
	//	ProduceTargetItem->bIsCopy = true;
	//}



	class UWrapBox* wrapBox = Cast<UWrapBox>(this->GetParent());
	if (wrapBox)
		wrapBox->RemoveChild(this);

	CheckWrapBox(wrapBox);
}