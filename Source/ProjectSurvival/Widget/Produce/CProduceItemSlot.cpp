#include "Widget/Produce/CProduceItemSlot.h"
#include "Widget/Produce/CProduceWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/CDebug.h"

bool UCProduceItemSlot::Initialize()
{
	bool Sucess = Super::Initialize();

	if (!Sucess)
		return false;

	if (!IsValid(ProduceItemSlotButton)) { CDebug::Print("ProduceItemSlotButton is invalid"); return false; }
	ProduceItemSlotButton->OnClicked.AddDynamic(this, &UCProduceItemSlot::ClickProduceSlotButton);

	return true;
}

void UCProduceItemSlot::SetProduceSlotIcon(UTexture2D* InTexture2D)
{
	ProduceItemSlotIcon->SetBrushFromTexture(InTexture2D);
}

void UCProduceItemSlot::SetProduceSlotID(FName InID)
{
	ItemID = InID;
}

void UCProduceItemSlot::ClickProduceSlotButton()
{
	UCProduceWidget* produceWidget = Cast<UCProduceWidget>(this->GetTypedOuter<UUserWidget>());
	if (produceWidget && ClickSound)
	{
		produceWidget->SetProduceDetail(ItemID, produceWidget->GetProducePanelSwitcherIndex(), produceWidget->GetWidgetCall());
		
		UGameplayStatics::PlaySound2D(this, ClickSound);
	}
	else
	{
		CDebug::Print("produceWidget is not Vaild");
	}

	this->GetParent()->GetTypedOuter<UUserWidget>()->SetFocus();
}
