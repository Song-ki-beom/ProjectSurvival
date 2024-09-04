#include "Widget/Produce/CProduceWidget.h"
#include "Character/CSurvivor.h"
#include "ActorComponents/CInventoryComponent.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/GridPanel.h"
#include "Struct/CItemDataStructures.h"
#include "Widget/Produce/CProduceDetail.h"
#include "Widget/Produce/CProduceItemSlot.h"
#include "Widget/Inventory/CItemBase.h"
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
	//return FReply::Unhandled();

	if (InKeyEvent.GetKey() == EKeys::E)
	{
		ProduceDetail->ProduceItem();
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

bool UCProduceWidget::Initialize()
{
	bool Sucess = Super::Initialize();

	if (!Sucess)
		return false;

	if (!GetWorld() || !IsValid(GetWorld()->GetFirstPlayerController())) { CDebug::Print("World or PlayerController is invalid"); return false; }

	Survivor = Cast<ACSurvivor>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (!IsValid(Survivor)) { CDebug::Print("Survivor is invalid"); return false; }

	InventoryComponent = Survivor->GetInventoryComponent();
	if (!IsValid(InventoryComponent)) { CDebug::Print("InventoryComponent is invalid"); return false; }

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

// ProduceSlot을 클릭했을때 상단에 출력되는 UI 설정
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
						inventoryQuantity = itemBase->Quantity;
						break;
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
						inventoryQuantity = itemBase->Quantity;
						break;
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
						inventoryQuantity = itemBase->Quantity;
						break;
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
						inventoryQuantity = itemBase->Quantity;
						break;
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
						inventoryQuantity = itemBase->Quantity;
						break;
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
