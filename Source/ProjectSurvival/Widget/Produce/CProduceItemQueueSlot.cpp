#include "Widget/Produce/CProduceItemQueueSlot.h"
#include "Widget/Produce/CProduceWidget.h"
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
		CDebug::Print("produceWidget is not valid");

	TotalProduceTime = FCString::Atof(*ProduceTimeText->GetText().ToString());
	RemainProduceTime = TotalProduceTime;
	GetWorld()->GetTimerManager().SetTimer(ProgressTimerHandle, this, &UCProduceItemQueueSlot::SetProduceProgress, 0.1f, true);
}

void UCProduceItemQueueSlot::SetProduceProgress()
{
	RemainProduceTime -= 0.1f;
	FText produceTimeText = FText::Format(FText::FromString(TEXT("{0}초")), FText::AsNumber(RemainProduceTime, &FNumberFormattingOptions().SetMaximumFractionalDigits(1)));
	ProduceTimeText->SetText(produceTimeText);

	float progress = (TotalProduceTime - RemainProduceTime) / TotalProduceTime;
	ProduceProgressBar->SetPercent(progress);

	if (RemainProduceTime <= 0.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(ProgressTimerHandle);
		EndProduce();
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
	class UWrapBox* wrapBox = Cast<UWrapBox>(this->GetParent());
	if (wrapBox)
		wrapBox->RemoveChild(this);

	CheckWrapBox(wrapBox);
}