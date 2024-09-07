#include "Widget/Inventory/CInventoryPanel_WorkingBench.h"
#include "Widget/Inventory/CItemDragDropOperation.h"
#include "Widget/Produce/CProduceWidget.h"
#include "Widget/CMainHUD.h"
#include "Components/WrapBox.h"
#include "Components/Button.h"
#include "Utility/CDebug.h"

bool UCInventoryPanel_WorkingBench::Initialize()
{
	bool Sucess = Super::Initialize();

	if (!Sucess)
		return false;

	if (!IsValid(TestButton)) { CDebug::Print("TestButton is invalid"); return false; }
	TestButton->OnClicked.AddDynamic(this, &UCInventoryPanel_WorkingBench::ChangeButtonColor);

	return true;
}

FReply UCInventoryPanel_WorkingBench::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);
	if (InKeyEvent.GetKey() == EKeys::I)
	{
		ACMainHUD* mainHUD = Cast<ACMainHUD>(this->GetOwningPlayer()->GetHUD());
		if (mainHUD)
			mainHUD->SetWidgetVisibility(EWidgetCall::CloseWidget);
		return FReply::Handled();
	}

	if (InKeyEvent.GetKey() == EKeys::E)
	{
		ACMainHUD* mainHUD = Cast<ACMainHUD>(GetOwningPlayer()->GetHUD());
		if (mainHUD)
			mainHUD->GetProduceWidget()->StartProduce();
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

bool UCInventoryPanel_WorkingBench::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	// 드롭이 어디서 시작됐는지 상태변수 필요할 거 같습니다

	const UCItemDragDropOperation* ItemDragDrop = Cast<UCItemDragDropOperation>(InOperation);
	if (ItemDragDrop->SourceItem) //아이템이 DragDropOperation에서 감지되면 (아이템이 Drag중이면 )
	{
		CDebug::Print("Drop");
		return true;
	}
	return false;
}

void UCInventoryPanel_WorkingBench::ChangeButtonColor()
{
	if (TestButton)
	{
		CDebug::Print("TestButton is Valid");
		TestButton->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
		CDebug::Print("TestButton is not Valid");
}

