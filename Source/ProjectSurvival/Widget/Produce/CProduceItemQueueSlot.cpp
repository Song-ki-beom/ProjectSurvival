#include "Widget/Produce/CProduceItemQueueSlot.h"
#include "Widget/Produce/CProduceWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
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

	UCProduceWidget* produceWidget = Cast<UCProduceWidget>(this->GetTypedOuter<UUserWidget>());
	if (produceWidget)
	{
		FText produceItemNameText = FText::Format(FText::FromString(TEXT("생산 중: {0}")), ProduceItemName);
		produceWidget->SetProducingItemText(produceItemNameText);
	}
}

void UCProduceItemQueueSlot::EndProduce()
{

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