#include "Widget/Produce/CProduceItemSlot.h"
#include "Components/Image.h"

void UCProduceItemSlot::SetProduceSlotIcon(UTexture2D* InTexture2D)
{
	ProduceItemSlotIcon->SetBrushFromTexture(InTexture2D);
}

