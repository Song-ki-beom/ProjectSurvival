#include "Widget/Inventory/CInventoryPanel_WorkingBench.h"
#include "Widget/Inventory/CItemDragDropOperation.h"
#include "Widget/Inventory/CItemBase.h"
#include "Widget/Inventory/CInventoryItemSlot.h"
#include "Character/CSurvivor.h"
#include "ActorComponents/CInventoryComponent.h"
//#include "Input/Reply.h"
#include "Components/WrapBox.h"
#include "Utility/CDebug.h"

bool UCInventoryPanel_WorkingBench::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	// 드롭이 어디서 시작됐는지 상태변수 필요할 거 같습니다

	const UCItemDragDropOperation* ItemDragDrop = Cast<UCItemDragDropOperation>(InOperation);
	if (ItemDragDrop->SourceItem) //아이템이 DragDropOperation에서 감지되면 (아이템이 Drag중이면 )
	{
		return true;
	}
	return false;
}

