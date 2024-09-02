#include "Widget/Produce/CProduceWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/GridPanel.h"
#include "Struct/CItemDataStructures.h"
#include "Widget/Produce/CProduceItemSlot.h"
#include "Utility/CDebug.h"

void UCProduceWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

FReply UCProduceWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);
	if (InKeyEvent.GetKey() == EKeys::I)
	{
		OnProduceWidgetToggled.Broadcast();  // 메뉴를 켜고 끄는 함수 호출
		return FReply::Handled();  // 입력을 처리했다고 반환
	}
	return FReply::Unhandled();
}

bool UCProduceWidget::Initialize()
{
	bool Sucess = Super::Initialize();

	if (!Sucess)
		return false;

	if (!IsValid(BuildStructureSelectButton)) { CDebug::Print("BuildStructureSelectButton is invalid"); return false; }
	BuildStructureSelectButton->OnClicked.AddDynamic(this, &UCProduceWidget::ClickBuildStructureButton);

	if (!IsValid(ToolSelectButton)) { CDebug::Print("ToolSelectButton is invalid"); return false; }
	ToolSelectButton->OnClicked.AddDynamic(this, &UCProduceWidget::ClickToolButton);

	if (!IsValid(WeaponSelectButton)) { CDebug::Print("WeaponSelectButton is invalid"); return false; }
	WeaponSelectButton->OnClicked.AddDynamic(this, &UCProduceWidget::ClickWeaponButton);

	ItemData = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, TEXT("DataTable'/Game/PirateIsland/Include/Datas/Widget/Inventory/DT_Items.DT_Items'")));
	if (IsValid(ItemData))
		CDebug::Print("ItemData is Valid");
	else
		CDebug::Print("ItemData is not Valid");

	int32 itemNumber = 1;

	for (UWidget* buildChildren : BuildPanel->GetAllChildren())
	{

		UCProduceItemSlot* produceItemSlot = Cast<UCProduceItemSlot>(buildChildren);
		if (IsValid(produceItemSlot))
		{
			FName itemRowName = FName(*FString::Printf(TEXT("Build_%d"), itemNumber));
			FItemData* itemData = ItemData->FindRow<FItemData>(itemRowName, TEXT(""));
			if (itemData)
			{
				CDebug::Print("Valid");
				UTexture2D* itemIcon = itemData->AssetData.Icon;
				produceItemSlot->SetProduceSlotIcon(itemIcon);
				CDebug::Print("Texture : ", itemIcon);
				itemNumber++;
			}
			else
				CDebug::Print("Not Valid");
		}

	}

	CDebug::Print("Initialize Success");

	return true;
}

void UCProduceWidget::ClickBuildStructureButton()
{
	if (IsValid(ProducePanelSwitcher))
	{
		ProducePanelSwitcher->SetActiveWidgetIndex(0);
		CDebug::Print("SetActiveWidgetIndex - 0");
	}
	else
	{
		CDebug::Print("ProducePanelSwitcher is not Valid");
	}
	//CDebug::Print("SetActiveWidgetIndex - 0");
	//ProducePanelSwitcher->SetActiveWidgetIndex(0);
}

void UCProduceWidget::ClickToolButton()
{
	if (IsValid(ProducePanelSwitcher))
	{
		ProducePanelSwitcher->SetActiveWidgetIndex(1);
		CDebug::Print("SetActiveWidgetIndex - 1");
	}
	else
	{
		CDebug::Print("ProducePanelSwitcher is not Valid");
	}
	//CDebug::Print("SetActiveWidgetIndex - 1");
	//ProducePanelSwitcher->SetActiveWidgetIndex(1);
}

void UCProduceWidget::ClickWeaponButton()
{
	if (IsValid(ProducePanelSwitcher))
	{
		ProducePanelSwitcher->SetActiveWidgetIndex(2);
		CDebug::Print("SetActiveWidgetIndex - 2");
	}
	else
	{
		CDebug::Print("ProducePanelSwitcher is not Valid");
	}
	//CDebug::Print("SetActiveWidgetIndex - 2");
	//ProducePanelSwitcher->SetActiveWidgetIndex(2);
}
