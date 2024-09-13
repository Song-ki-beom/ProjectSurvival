#include "Widget/Produce/CProduceWidget.h"
#include "Character/CSurvivor.h"
#include "ActorComponents/CInventoryComponent.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/GridPanel.h"
#include "Components/WrapBox.h"
#include "Struct/CItemDataStructures.h"
#include "Widget/Produce/CProduceDetail.h"
#include "Widget/Produce/CProduceItemSlot.h"
#include "Widget/Produce/CProduceItemQueueSlot.h"
#include "Widget/Inventory/CItemBase.h"
#include "Widget/Chatting/CChattingBox.h"
#include "Widget/CMainHUD.h"
#include "CGameInstance.h"
#include "Utility/CDebug.h"

void UCProduceWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Survivor = Cast<ACSurvivor>(GetOwningPlayerPawn());
	if (Survivor)
		InventoryComponent = Survivor->GetInventoryComponent();
}

FReply UCProduceWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);
	if (InKeyEvent.GetKey() == EKeys::I)
	{
		ACMainHUD* mainHUD = Cast<ACMainHUD>(GetOwningPlayer()->GetHUD());
		if (mainHUD)
			mainHUD->SetWidgetVisibility(EWidgetCall::CloseWidget);
		return FReply::Handled();  // 입력을 처리했다고 반환
	}

	//return FReply::Unhandled();

	if (InKeyEvent.GetKey() == EKeys::E)
	{
		StartProduce();
		return FReply::Handled();
	}

	if (InKeyEvent.GetKey() == EKeys::Enter)
	{
		UCGameInstance* gameInstance = Cast<UCGameInstance>(GetGameInstance());
		if (gameInstance)
		{
			CDebug::Print("CGameInstance is Valid", gameInstance);
			if (gameInstance->ChattingBox)
			{
				gameInstance->ChattingBox->SetInputMode();
			}
			else
				CDebug::Print("gameInstance->ChattingBox is not Valid");
		}
		else
			CDebug::Print("gameInstance is not Valid");

		return FReply::Handled();
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

	// 범위기반 for loop에 사용될 Build_ 뒤에 문자열로 추가할 정수형 변수
	int32 itemNumber = 1;

	// ProduceItemSlot에 텍스처 이미지 설정 및 ID 부여
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
				produceItemSlot->SetProduceSlotID(itemRowName);
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

void UCProduceWidget::SetWidgetSwitcherIndex(int32 InIndex)
{
	ProduceWidgetSwitcher->SetActiveWidgetIndex(InIndex);
}

void UCProduceWidget::SetProduceDetail(FName InID)
{
	SelectedID = InID;
	FItemData* itemData = ItemData->FindRow<FItemData>(InID, TEXT(""));
	if (itemData)
	{
		UTexture2D* itemIcon = itemData->AssetData.Icon;
		if (itemIcon)
			ProduceDetail->SetProduceDetailIcon(itemIcon);
		else
			CDebug::Print("itemIcon is not Valid");

		FText itemName = itemData->TextData.Name;
		ProduceDetail->SetProduceDetailName(itemName);

		float itemWeight = itemData->NumericData.Weight;
		FText itemWeightText = FText::Format(FText::FromString(TEXT("무게: {0}kg")), FText::AsNumber(itemWeight));
		ProduceDetail->SetProduceDetailWeight(itemWeightText);

		float itemProduceTime = itemData->ProduceData.ProduceTime;
		FText itemProduceTimeText = FText::Format(FText::FromString(TEXT("제작 시간: {0}초")), FText::AsNumber(itemProduceTime));
		ProduceDetail->SetProduceDetailTime(itemProduceTimeText);

		FText itemProduceMethod = itemData->ProduceData.ProduceMethod;
		FText itemProduceMethodText = FText::Format(FText::FromString(TEXT("제작 방법: {0}")), itemProduceMethod);
		ProduceDetail->SetProduceDetailMethod(itemProduceMethodText);

		FText itemFlavorText = itemData->ProduceData.FlavorText;
		ProduceDetail->SetProduceDetailFlavorText(itemFlavorText);

		// 초기화 해서 요구 재료 스크롤 박스 자손 제거
		ProduceDetail->ClearRecipeScrollBox();

		// 조합식은 현재까지 넉넉하게 5가지만 생각 중. 더 다양하게 조합하고 싶을 경우 추가하면 됨
		if (itemData->ProduceData.ProduceResource_1.ResourceIcon)
		{
			int32 inventoryQuantity = 0;
			TArray<TWeakObjectPtr<UCItemBase>> itemArray = InventoryComponent->GetInventoryContents();
			for (TWeakObjectPtr<UCItemBase> itemBasePtr : itemArray)
			{
				if (UCItemBase* itemBase = itemBasePtr.Get())
				{
					if (itemBase->ID == itemData->ProduceData.ProduceResource_1.ResourceID)
					{
						inventoryQuantity += itemBase->Quantity;
					}
				}
			}
			FName resourceID = itemData->ProduceData.ProduceResource_1.ResourceID;
			UTexture2D* resourceIcon = itemData->ProduceData.ProduceResource_1.ResourceIcon;
			FText resourceName = itemData->ProduceData.ProduceResource_1.ResourceName;
			int32 demandQuantity = itemData->ProduceData.ProduceResource_1.ProduceResourceDemand;
			ProduceDetail->AddResourceToProduceRecipeScroll(resourceID, resourceIcon, resourceName, inventoryQuantity, demandQuantity);
		}

		if (itemData->ProduceData.ProduceResource_2.ResourceIcon)
		{
			int32 inventoryQuantity = 0;
			TArray<TWeakObjectPtr<UCItemBase>> itemArray = InventoryComponent->GetInventoryContents();
			for (TWeakObjectPtr<UCItemBase> itemBasePtr : itemArray)
			{
				if (UCItemBase* itemBase = itemBasePtr.Get())
				{
					if (itemBase->ID == itemData->ProduceData.ProduceResource_2.ResourceID)
					{
						inventoryQuantity += itemBase->Quantity;
					}
				}
			}
			FName resourceID = itemData->ProduceData.ProduceResource_2.ResourceID;
			UTexture2D* resourceIcon = itemData->ProduceData.ProduceResource_2.ResourceIcon;
			FText resourceName = itemData->ProduceData.ProduceResource_2.ResourceName;
			int32 demandQuantity = itemData->ProduceData.ProduceResource_2.ProduceResourceDemand;
			ProduceDetail->AddResourceToProduceRecipeScroll(resourceID, resourceIcon, resourceName, inventoryQuantity, demandQuantity);
		}

		if (itemData->ProduceData.ProduceResource_3.ResourceIcon)
		{
			int32 inventoryQuantity = 0;
			TArray<TWeakObjectPtr<UCItemBase>> itemArray = InventoryComponent->GetInventoryContents();
			for (TWeakObjectPtr<UCItemBase> itemBasePtr : itemArray)
			{
				if (UCItemBase* itemBase = itemBasePtr.Get())
				{
					if (itemBase->ID == itemData->ProduceData.ProduceResource_3.ResourceID)
					{
						inventoryQuantity += itemBase->Quantity;
					}
				}
			}
			FName resourceID = itemData->ProduceData.ProduceResource_3.ResourceID;
			UTexture2D* resourceIcon = itemData->ProduceData.ProduceResource_3.ResourceIcon;
			FText resourceName = itemData->ProduceData.ProduceResource_3.ResourceName;
			int32 demandQuantity = itemData->ProduceData.ProduceResource_3.ProduceResourceDemand;
			ProduceDetail->AddResourceToProduceRecipeScroll(resourceID, resourceIcon, resourceName, inventoryQuantity, demandQuantity);
		}
		if (itemData->ProduceData.ProduceResource_4.ResourceIcon)
		{
			int32 inventoryQuantity = 0;
			TArray<TWeakObjectPtr<UCItemBase>> itemArray = InventoryComponent->GetInventoryContents();
			for (TWeakObjectPtr<UCItemBase> itemBasePtr : itemArray)
			{
				if (UCItemBase* itemBase = itemBasePtr.Get())
				{
					if (itemBase->ID == itemData->ProduceData.ProduceResource_4.ResourceID)
					{
						inventoryQuantity += itemBase->Quantity;
					}
				}
			}
			FName resourceID = itemData->ProduceData.ProduceResource_4.ResourceID;
			UTexture2D* resourceIcon = itemData->ProduceData.ProduceResource_4.ResourceIcon;
			FText resourceName = itemData->ProduceData.ProduceResource_4.ResourceName;
			int32 demandQuantity = itemData->ProduceData.ProduceResource_4.ProduceResourceDemand;
			ProduceDetail->AddResourceToProduceRecipeScroll(resourceID, resourceIcon, resourceName, inventoryQuantity, demandQuantity);
		}
		if (itemData->ProduceData.ProduceResource_5.ResourceIcon)
		{
			int32 inventoryQuantity = 0;
			TArray<TWeakObjectPtr<UCItemBase>> itemArray = InventoryComponent->GetInventoryContents();
			for (TWeakObjectPtr<UCItemBase> itemBasePtr : itemArray)
			{
				if (UCItemBase* itemBase = itemBasePtr.Get())
				{
					if (itemBase->ID == itemData->ProduceData.ProduceResource_5.ResourceID)
					{
						inventoryQuantity += itemBase->Quantity;
					}
				}
			}
			FName resourceID = itemData->ProduceData.ProduceResource_5.ResourceID;
			UTexture2D* resourceIcon = itemData->ProduceData.ProduceResource_5.ResourceIcon;
			FText resourceName = itemData->ProduceData.ProduceResource_5.ResourceName;
			int32 demandQuantity = itemData->ProduceData.ProduceResource_5.ProduceResourceDemand;
			ProduceDetail->AddResourceToProduceRecipeScroll(resourceID, resourceIcon, resourceName, inventoryQuantity, demandQuantity);
		}
	}
	else
		CDebug::Print("itemData is not Valid");
}

void UCProduceWidget::RefreshProduceDetail()
{
	if (!SelectedID.IsNone())
		SetProduceDetail(SelectedID);
	else
	{
		FName initialID = "Build_1";
		SetProduceDetail(initialID);
	}
}

void UCProduceWidget::StartProduce()
{
	if (ProduceQueue->GetChildrenCount() > 5)
	{
		// 대기열 가득참
		return;
	}

	ProduceDetail->ProduceItem();
}

void UCProduceWidget::AddProduceItemToQueue()
{
	UClass* widgetClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/PirateIsland/Include/Blueprints/Widget/Produce/WBP_CProduceItemQueueSlot.WBP_CProduceItemQueueSlot_C'"));
	if (widgetClass)
	{
		UCProduceItemQueueSlot* produceItemQueueSlot = CreateWidget<UCProduceItemQueueSlot>(GetWorld(), widgetClass);
		if (produceItemQueueSlot)
		{
			FItemData* itemData = ItemData->FindRow<FItemData>(SelectedID, TEXT(""));
			if (itemData)
			{
				FName itemID = itemData->ID;
				produceItemQueueSlot->SetProduceItemID(itemID);

				UTexture2D* itemIcon = itemData->AssetData.Icon;
				produceItemQueueSlot->SetProduceQueueSlotIcon(itemIcon);
				
				int32 produceTime = itemData->ProduceData.ProduceTime;
				FText produceTimeText = FText::Format(FText::FromString(TEXT("{0}초")), FText::AsNumber(produceTime));
				produceItemQueueSlot->SetProduceTimeText(produceTimeText);
				
				FText produceItemName = itemData->TextData.Name;
				produceItemQueueSlot->SetProduceItemName(produceItemName);

				FProduceWidgetData produceWidgetData = itemData->ProduceData;
				produceItemQueueSlot->SetProduceWidgetData(produceWidgetData);

				ProduceQueue->AddChildToWrapBox(produceItemQueueSlot);
				produceItemQueueSlot->CheckWrapBox(ProduceQueue);
			}
		}
	}
}

void UCProduceWidget::SetProducingItemText(FText InText, FLinearColor InLinearColor)
{
	ProducingItemText->SetText(InText);
	ProducingItemText->SetColorAndOpacity(FSlateColor(InLinearColor));
}

//void UCProduceWidget::Test_ShowPlaceableInventory()
//{
//	UClass* widgetClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/PirateIsland/Include/Blueprints/Widget/Inventory/WBP_CInventoryPanel_WorkingBench.WBP_CInventoryPanel_WorkingBench_C'"));
//	
//	if (widgetClass)
//	{
//		UCInventoryPanel_WorkingBench* workingBenchPanelWidget = CreateWidget<UCInventoryPanel_WorkingBench>(GetWorld(), widgetClass);
//		if (workingBenchPanelWidget)
//		{
//			workingBenchPanelWidget->AddToViewport(5);
//			//ProduceWidget->SetVisibility(ESlateVisibility::Visible);
//			//ProduceWidget->RefreshProduceDetail();
//			//ProduceWidget->bIsFocusable = true;
//		}
//	}
//}

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
