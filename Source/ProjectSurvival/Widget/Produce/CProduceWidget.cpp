#include "Widget/Produce/CProduceWidget.h"
#include "Character/CSurvivor.h"
#include "Character/CSurvivorController.h"
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
#include "Widget/Inventory/CInventoryPanel_Placeable.h"
#include "Widget/Chatting/CChattingBox.h"
#include "Widget/CMainHUD.h"
#include "Build/CStructure_Placeable.h"
#include "CGameInstance.h"
#include "Utility/CDebug.h"

void UCProduceWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Survivor = Cast<ACSurvivor>(GetOwningPlayerPawn());
	if (Survivor)
		InventoryComponent = Survivor->GetInventoryComponent();

	UClass* produceItemSlotClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/PirateIsland/Include/Blueprints/Widget/Produce/WBP_CProduceItemSlot.WBP_CProduceItemSlot_C'"));

	switch (WidgetCall)
	{
	case EWidgetCall::Survivor:
	{
		if (Survivor)
		{
			SetProduceWindowName(FText::FromString(TEXT("제작 - 생존자")));
			CreateBuildProduceItemSlot(1, 16);
			CreateToolProduceItemSlot(1, 2);
			CreateWeaponProduceItemSlot(3, 4);
			IgniteButton->GetParent()->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
			CDebug::Print("Survivor is Not Valid");
		break;
	}
	case EWidgetCall::Placeable:
	{
		CDebug::Print("Ignite Button Brush Saved");
		IgniteButtonNormalBrush = IgniteButton->WidgetStyle.Normal;
		IgniteButtonPressedBrush = IgniteButton->WidgetStyle.Pressed;
		RefreshProduceDetail();
		break;
	}
	}


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

	if (!IsValid(IgniteButton)) { CDebug::Print("IgniteButton is invalid"); return false; }
	IgniteButton->OnClicked.AddDynamic(this, &UCProduceWidget::ClickIgniteButton);

	ItemData = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, TEXT("DataTable'/Game/PirateIsland/Include/Datas/Widget/Inventory/DT_Items.DT_Items'")));
	if (!IsValid(ItemData))
		CDebug::Print("ItemData is not Valid");

	return true;
}

void UCProduceWidget::SetProduceWindowName(FText InText)
{
	ProduceWindowName->SetText(InText);
}

void UCProduceWidget::SetButtonVisivility(ESlateVisibility BuildVisibility, ESlateVisibility ToolVisibility, ESlateVisibility WeaponVisibility, ESlateVisibility IgniteVisibility)
{
	BuildStructureSelectButton->GetParent()->SetVisibility(BuildVisibility);
	ToolSelectButton->GetParent()->SetVisibility(ToolVisibility);
	WeaponSelectButton->GetParent()->SetVisibility(WeaponVisibility);
	IgniteButton->GetParent()->SetVisibility(IgniteVisibility);
}

void UCProduceWidget::SetProducePanelSwitcherIndex(int32 InIndex)
{
	if (ProducePanelSwitcher)
		ProducePanelSwitcher->SetActiveWidgetIndex(InIndex);
}

void UCProduceWidget::CreateBuildProduceItemSlot(int32 StartIndex, int32 EndIndex)
{
	UClass* produceItemSlotClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/PirateIsland/Include/Blueprints/Widget/Produce/WBP_CProduceItemSlot.WBP_CProduceItemSlot_C'"));
	
	// DT_Items 변동사항 발생 시 수정 필요, Build 관련 ProduceItemSlot추가
	for (int32 i = StartIndex; i <= EndIndex; i++)
	{
		UCProduceItemSlot* produceItemSlot = CreateWidget<UCProduceItemSlot>(this, produceItemSlotClass);
		if (produceItemSlot)
		{
			FName itemRowName = FName(*FString::Printf(TEXT("Build_%d"), i));
			FItemData* itemData = ItemData->FindRow<FItemData>(itemRowName, TEXT(""));
			if (itemData)
			{
				UTexture2D* itemIcon = itemData->AssetData.Icon;
				produceItemSlot->SetProduceSlotIcon(itemIcon);
				produceItemSlot->SetProduceSlotID(itemRowName);
				BuildPanel->AddChild(produceItemSlot);
			}
			else
				CDebug::Print("itemData is Not Valid");

		}
		else
			CDebug::Print("produceItemSlot is Not Valid");
	}
}

void UCProduceWidget::CreateToolProduceItemSlot(int32 StartIndex, int32 EndIndex)
{
	UClass* produceItemSlotClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/PirateIsland/Include/Blueprints/Widget/Produce/WBP_CProduceItemSlot.WBP_CProduceItemSlot_C'"));

	// DT_Items 변동사항 발생 시 수정 필요, Hunt(Tool) 관련 ProduceItemSlot추가
	for (int32 i = StartIndex; i <= EndIndex; i++)
	{
		UCProduceItemSlot* produceItemSlot = CreateWidget<UCProduceItemSlot>(this, produceItemSlotClass);
		if (produceItemSlot)
		{
			FName itemRowName = FName(*FString::Printf(TEXT("Hunt_%d"), i));
			FItemData* itemData = ItemData->FindRow<FItemData>(itemRowName, TEXT(""));
			if (itemData)
			{
				UTexture2D* itemIcon = itemData->AssetData.Icon;
				produceItemSlot->SetProduceSlotIcon(itemIcon);
				produceItemSlot->SetProduceSlotID(itemRowName);
				ToolPanel->AddChild(produceItemSlot);
			}
			else
				CDebug::Print("itemData is Not Valid");

		}
		else
			CDebug::Print("produceItemSlot is Not Valid");
	}
}

void UCProduceWidget::CreateWeaponProduceItemSlot(int32 StartIndex, int32 EndIndex)
{
	UClass* produceItemSlotClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/PirateIsland/Include/Blueprints/Widget/Produce/WBP_CProduceItemSlot.WBP_CProduceItemSlot_C'"));

	// DT_Items 변동사항 발생 시 수정 필요, Hunt(Weapon) 관련 ProduceItemSlot추가
	for (int32 i = StartIndex; i <= EndIndex; i++)
	{
		UCProduceItemSlot* produceItemSlot = CreateWidget<UCProduceItemSlot>(this, produceItemSlotClass);
		if (produceItemSlot)
		{
			FName itemRowName = FName(*FString::Printf(TEXT("Hunt_%d"), i));
			FItemData* itemData = ItemData->FindRow<FItemData>(itemRowName, TEXT(""));
			if (itemData)
			{
				UTexture2D* itemIcon = itemData->AssetData.Icon;
				produceItemSlot->SetProduceSlotIcon(itemIcon);
				produceItemSlot->SetProduceSlotID(itemRowName);
				WeaponPanel->AddChild(produceItemSlot);
			}
			else
				CDebug::Print("itemData is Not Valid");

		}
		else
			CDebug::Print("produceItemSlot is Not Valid");
	}
}

void UCProduceWidget::CreateHarvestProduceItemSlot(int32 StartIndex, int32 EndIndex)
{
	UClass* produceItemSlotClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/PirateIsland/Include/Blueprints/Widget/Produce/WBP_CProduceItemSlot.WBP_CProduceItemSlot_C'"));

	// DT_Items 변동사항 발생 시 수정 필요, Harvest 관련 ProduceItemSlot추가
	for (int32 i = StartIndex; i <= EndIndex; i++)
	{
		UCProduceItemSlot* produceItemSlot = CreateWidget<UCProduceItemSlot>(this, produceItemSlotClass);
		if (produceItemSlot)
		{
			FName itemRowName = FName(*FString::Printf(TEXT("Harvest_%d"), i));
			FItemData* itemData = ItemData->FindRow<FItemData>(itemRowName, TEXT(""));
			if (itemData)
			{
				UTexture2D* itemIcon = itemData->AssetData.Icon;
				produceItemSlot->SetProduceSlotIcon(itemIcon);
				produceItemSlot->SetProduceSlotID(itemRowName);
				HarvestPanel->AddChild(produceItemSlot);
			}
			else
				CDebug::Print("itemData is Not Valid");

		}
		else
			CDebug::Print("produceItemSlot is Not Valid");
	}
}

void UCProduceWidget::SetProduceDetail(FName InID, int32 InIndex, EWidgetCall InWidgetCall)
{
	FItemData* itemData = nullptr;

	switch (InIndex)
	{
	case 0:
		SelectedBuildID = InID;
		itemData = ItemData->FindRow<FItemData>(SelectedBuildID, TEXT(""));
		break;
	case 1:
		SelectedToolID = InID;
		itemData = ItemData->FindRow<FItemData>(SelectedToolID, TEXT(""));
		break;
	case 2:
		SelectedWeaponID = InID;
		itemData = ItemData->FindRow<FItemData>(SelectedWeaponID, TEXT(""));
		break;
	case 3:
		SelectedWeaponID = InID;
		itemData = ItemData->FindRow<FItemData>(SelectedHarvestID, TEXT(""));
	default:
		break;
	}

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
		switch (InWidgetCall)
		{
		case EWidgetCall::Survivor:
		{
			FText workingBenchItemProduceTimeText = FText::Format(FText::FromString(TEXT("제작 시간: {0}초")), FText::AsNumber(itemProduceTime));
			ProduceDetail->SetProduceDetailTime(workingBenchItemProduceTimeText);
			break;
		}
		case EWidgetCall::Placeable:
		{
			switch (OwnerActor->GetPlaceableStructureType())
			{
				case EPlaceableStructureType::WorkingBench:
				{
					if (itemData->ProduceData.bApplyRichText)
					{
						FText workingBenchItemProduceTimeText = FText::Format(FText::FromString(TEXT("제작 시간: <GrayStrike>{0}초</> -> <Green>{1}초</>")), FText::AsNumber(itemProduceTime), FText::AsNumber(itemProduceTime / 2));
						ProduceDetail->SetProduceDetailTime(workingBenchItemProduceTimeText);
					}
					else
					{
						FText workingBenchItemProduceTimeText = FText::Format(FText::FromString(TEXT("제작 시간: {0}초")), FText::AsNumber(itemProduceTime));
						ProduceDetail->SetProduceDetailTime(workingBenchItemProduceTimeText);
					}
					break;
				}
				default:
				{
					FText workingBenchItemProduceTimeText = FText::Format(FText::FromString(TEXT("제작 시간: {0}초")), FText::AsNumber(itemProduceTime));
					ProduceDetail->SetProduceDetailTime(workingBenchItemProduceTimeText);
					break;
				}
			}

		}
		default:
		{
			break;
		}
		}
		FText itemProduceMethod = itemData->ProduceData.ProduceMethod;
		FText itemProduceMethodText = FText::Format(FText::FromString(TEXT("제작 방법: {0}")), itemProduceMethod);
		ProduceDetail->SetProduceDetailMethod(itemProduceMethodText);

		FText itemFlavorText = itemData->ProduceData.FlavorText;
		ProduceDetail->SetProduceDetailFlavorText(itemFlavorText);

		// 초기화 해서 요구 재료 스크롤 박스 자손 제거
		ProduceDetail->ClearRecipeScrollBox();



		// 조합식은 현재까지 넉넉하게 5가지만 생각 중. 더 다양하게 조합하고 싶을 경우 추가하면 됨
		switch (InWidgetCall)
		{
		case EWidgetCall::Survivor:
		{
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
			break;
		}
		case EWidgetCall::Placeable:
		{
			if (itemData->ProduceData.ProduceResource_1.ResourceIcon)
			{
				int32 placeableInventoryQuantity = 0;
				TArray<FItemInformation> itemInfoArray = OwnerActor->GetItemInfoArray();

				for (FItemInformation itemInfo : itemInfoArray)
				{
					if (itemInfo.ItemID == itemData->ProduceData.ProduceResource_1.ResourceID)
					{
						placeableInventoryQuantity += itemInfo.Quantity;
					}
				}
				FName resourceID = itemData->ProduceData.ProduceResource_1.ResourceID;
				UTexture2D* resourceIcon = itemData->ProduceData.ProduceResource_1.ResourceIcon;
				FText resourceName = itemData->ProduceData.ProduceResource_1.ResourceName;
				int32 demandQuantity = itemData->ProduceData.ProduceResource_1.ProduceResourceDemand;
				ProduceDetail->AddResourceToProduceRecipeScroll(resourceID, resourceIcon, resourceName, placeableInventoryQuantity, demandQuantity);
			}

			if (itemData->ProduceData.ProduceResource_2.ResourceIcon)
			{
				int32 placeableInventoryQuantity = 0;
				TArray<FItemInformation> itemInfoArray = OwnerActor->GetItemInfoArray();

				for (FItemInformation itemInfo : itemInfoArray)
				{
					if (itemInfo.ItemID == itemData->ProduceData.ProduceResource_2.ResourceID)
					{
						placeableInventoryQuantity += itemInfo.Quantity;
					}
				}
				FName resourceID = itemData->ProduceData.ProduceResource_2.ResourceID;
				UTexture2D* resourceIcon = itemData->ProduceData.ProduceResource_2.ResourceIcon;
				FText resourceName = itemData->ProduceData.ProduceResource_2.ResourceName;
				int32 demandQuantity = itemData->ProduceData.ProduceResource_2.ProduceResourceDemand;
				ProduceDetail->AddResourceToProduceRecipeScroll(resourceID, resourceIcon, resourceName, placeableInventoryQuantity, demandQuantity);
			}

			if (itemData->ProduceData.ProduceResource_3.ResourceIcon)
			{
				int32 placeableInventoryQuantity = 0;
				TArray<FItemInformation> itemInfoArray = OwnerActor->GetItemInfoArray();

				for (FItemInformation itemInfo : itemInfoArray)
				{
					if (itemInfo.ItemID == itemData->ProduceData.ProduceResource_2.ResourceID)
					{
						placeableInventoryQuantity += itemInfo.Quantity;
					}
				}
				FName resourceID = itemData->ProduceData.ProduceResource_3.ResourceID;
				UTexture2D* resourceIcon = itemData->ProduceData.ProduceResource_3.ResourceIcon;
				FText resourceName = itemData->ProduceData.ProduceResource_3.ResourceName;
				int32 demandQuantity = itemData->ProduceData.ProduceResource_3.ProduceResourceDemand;
				ProduceDetail->AddResourceToProduceRecipeScroll(resourceID, resourceIcon, resourceName, placeableInventoryQuantity, demandQuantity);
			}

			if (itemData->ProduceData.ProduceResource_4.ResourceIcon)
			{
				int32 placeableInventoryQuantity = 0;
				TArray<FItemInformation> itemInfoArray = OwnerActor->GetItemInfoArray();

				for (FItemInformation itemInfo : itemInfoArray)
				{
					if (itemInfo.ItemID == itemData->ProduceData.ProduceResource_4.ResourceID)
					{
						placeableInventoryQuantity += itemInfo.Quantity;
					}
				}
				FName resourceID = itemData->ProduceData.ProduceResource_4.ResourceID;
				UTexture2D* resourceIcon = itemData->ProduceData.ProduceResource_4.ResourceIcon;
				FText resourceName = itemData->ProduceData.ProduceResource_4.ResourceName;
				int32 demandQuantity = itemData->ProduceData.ProduceResource_4.ProduceResourceDemand;
				ProduceDetail->AddResourceToProduceRecipeScroll(resourceID, resourceIcon, resourceName, placeableInventoryQuantity, demandQuantity);
			}

			if (itemData->ProduceData.ProduceResource_5.ResourceIcon)
			{
				int32 placeableInventoryQuantity = 0;
				TArray<FItemInformation> itemInfoArray = OwnerActor->GetItemInfoArray();

				for (FItemInformation itemInfo : itemInfoArray)
				{
					if (itemInfo.ItemID == itemData->ProduceData.ProduceResource_5.ResourceID)
					{
						placeableInventoryQuantity += itemInfo.Quantity;
					}
				}
				FName resourceID = itemData->ProduceData.ProduceResource_5.ResourceID;
				UTexture2D* resourceIcon = itemData->ProduceData.ProduceResource_5.ResourceIcon;
				FText resourceName = itemData->ProduceData.ProduceResource_5.ResourceName;
				int32 demandQuantity = itemData->ProduceData.ProduceResource_5.ProduceResourceDemand;
				ProduceDetail->AddResourceToProduceRecipeScroll(resourceID, resourceIcon, resourceName, placeableInventoryQuantity, demandQuantity);
			}

			break;
		}
		default:
		{
			break;
		}
		}



		
	}
	else
		CDebug::Print("itemData is not Valid");
}

void UCProduceWidget::RefreshProduceDetail()
{
	CDebug::Print("KKKKKKKKKKKKKKKKKKKK");
	switch (ProducePanelSwitcher->GetActiveWidgetIndex())
	{
	case 0:
		if (!SelectedBuildID.IsNone())
			SetProduceDetail(SelectedBuildID, 0, WidgetCall);
		else
		{
			SelectedBuildID = "Build_1";
			SetProduceDetail(SelectedBuildID, 0, WidgetCall);
		}
		break;
	case 1:
		if (!SelectedToolID.IsNone())
			SetProduceDetail(SelectedToolID, 1, WidgetCall);
		else
		{
			SelectedToolID = "Hunt_1";
			SetProduceDetail(SelectedToolID, 1, WidgetCall);
		}
		break;
	case 2:
		if (!SelectedWeaponID.IsNone())
			SetProduceDetail(SelectedWeaponID, 2, WidgetCall);
		else
		{
			SelectedWeaponID = "Hunt_3";
			SetProduceDetail(SelectedWeaponID, 2, WidgetCall);
		}
		break;
	case 3:
		if (!SelectedHarvestID.IsNone())
			SetProduceDetail(SelectedHarvestID, 3, WidgetCall);
		else
		{
			SelectedHarvestID = "Harvest_5";
			SetProduceDetail(SelectedHarvestID, 3, WidgetCall);
		}
	default:
		break;
	}
}

void UCProduceWidget::StartProduce()
{
	if (ProduceQueue->GetChildrenCount() > 5)
	{
		// 대기열 가득참
		return;
	}

	switch (WidgetCall)
	{
	case EWidgetCall::Survivor:
		switch (ProducePanelSwitcher->GetActiveWidgetIndex())
		{
		case 0:
			ProduceDetail->ProduceSurvivorItem(SelectedBuildID);
			break;
		case 1:
			ProduceDetail->ProduceSurvivorItem(SelectedToolID);
			break;
		case 2:
			ProduceDetail->ProduceSurvivorItem(SelectedWeaponID);
			break;
		default:
			break;
		}
		break;
	case EWidgetCall::Placeable:
		switch (ProducePanelSwitcher->GetActiveWidgetIndex())
		{
		case 0:
			ProduceDetail->ProducePlaceableItem(SelectedBuildID, OwnerActor);
			break;
		case 1:
			ProduceDetail->ProducePlaceableItem(SelectedToolID, OwnerActor);
			break;
		case 2:
			ProduceDetail->ProducePlaceableItem(SelectedWeaponID, OwnerActor);
			break;
		case 3:
			ProduceDetail->ProducePlaceableItem(SelectedHarvestID, OwnerActor);
			break;
		default:
			break;
		}
		break;
	}
}

void UCProduceWidget::AddProduceItemToQueue(FName InID)
{
	UClass* widgetClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/PirateIsland/Include/Blueprints/Widget/Produce/WBP_CProduceItemQueueSlot.WBP_CProduceItemQueueSlot_C'"));
	if (widgetClass)
	{
		UCProduceItemQueueSlot* produceItemQueueSlot = CreateWidget<UCProduceItemQueueSlot>(GetWorld(), widgetClass);
		if (produceItemQueueSlot)
		{
			FItemData* itemData = ItemData->FindRow<FItemData>(InID, TEXT(""));
			if (itemData)
			{
				FName itemID = itemData->ID;
				produceItemQueueSlot->SetProduceItemID(itemID);

				UTexture2D* itemIcon = itemData->AssetData.Icon;
				produceItemQueueSlot->SetProduceQueueSlotIcon(itemIcon);
				
				int32 produceTime = itemData->ProduceData.ProduceTime;
				
				if (OwnerActor)
				{
					switch (OwnerActor->GetPlaceableStructureType())
					{
						case EPlaceableStructureType::WorkingBench:
						{
							FText produceTimeText = FText::Format(FText::FromString(TEXT("{0}초")), FText::AsNumber(produceTime / 2));
							produceItemQueueSlot->SetProduceTimeText(produceTimeText);
							break;
						}
						default:
						{
							FText produceTimeText = FText::Format(FText::FromString(TEXT("{0}초")), FText::AsNumber(produceTime));
							produceItemQueueSlot->SetProduceTimeText(produceTimeText);
							break;
						}
					}
				}
				else
				{
					FText produceTimeText = FText::Format(FText::FromString(TEXT("{0}초")), FText::AsNumber(produceTime));
					produceItemQueueSlot->SetProduceTimeText(produceTimeText);
				}
				
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

int32 UCProduceWidget::GetProducePanelSwitcherIndex()
{
	return ProducePanelSwitcher->GetActiveWidgetIndex();
}

//void UCProduceWidget::Test_ShowPlaceableInventory()
//{
//	UClass* widgetClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/PirateIsland/Include/Blueprints/Widget/Inventory/WBP_CInventoryPanel_WorkingBench.WBP_CInventoryPanel_WorkingBench_C'"));
//	
//	if (widgetClass)
//	{
//		UCInventoryPanel_Placeable* workingBenchPanelWidget = CreateWidget<UCInventoryPanel_Placeable>(GetWorld(), widgetClass);
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
		RefreshProduceDetail();
	}
	else
		CDebug::Print("ProducePanelSwitcher is not Valid");

	BuildStructureSelectButton->GetTypedOuter<UUserWidget>()->SetFocus();
}

void UCProduceWidget::ClickToolButton()
{
	if (IsValid(ProducePanelSwitcher))
	{
		ProducePanelSwitcher->SetActiveWidgetIndex(1);
		RefreshProduceDetail();
	}
	else
		CDebug::Print("ProducePanelSwitcher is not Valid");

	ToolSelectButton->GetTypedOuter<UUserWidget>()->SetFocus();
}

void UCProduceWidget::ClickWeaponButton()
{
	if (IsValid(ProducePanelSwitcher))
	{
		ProducePanelSwitcher->SetActiveWidgetIndex(2);
		RefreshProduceDetail();
	}
	else
		CDebug::Print("ProducePanelSwitcher is not Valid");

	WeaponSelectButton->GetTypedOuter<UUserWidget>()->SetFocus();
}

void UCProduceWidget::ClickIgniteButton()
{
	if (OwnerActor->GetIgniteState())
	{
		// 화로가 켜져있는 상태일때
		FButtonStyle igniteButtonStyle = IgniteButton->WidgetStyle;
		igniteButtonStyle.Normal = IgniteButtonNormalBrush;
		igniteButtonStyle.Hovered = IgniteButtonNormalBrush;
		igniteButtonStyle.Pressed = IgniteButtonPressedBrush;
		IgniteButton->SetStyle(igniteButtonStyle);
		OwnerActor->SetIgniteState(false);
	}
	else
	{
		// 화로가 꺼져있는 상태일때
		for (UCItemBase* widgetItem : OwnerActor->GetPlaceableInventoryWidget()->GetWidgetItems())
		{
			if (widgetItem->ID == "Harvest_1")
			{
				if (widgetItem->Quantity == 1)
					OwnerActor->GetPlaceableInventoryWidget()->RemoveItem(widgetItem);
				else
					OwnerActor->GetPlaceableInventoryWidget()->RemoveAmountOfItem(widgetItem, 1);

				if (this->GetOwningPlayer()->HasAuthority())
				{
					OwnerActor->BroadcastSpawnFire();
				}
				else
				{
					ACSurvivorController* survivorController = Cast<ACSurvivorController>(this->GetOwningPlayer());
					if (survivorController)
						survivorController->RequestSpawnFire(OwnerActor);
				}
				FButtonStyle igniteButtonStyle = IgniteButton->WidgetStyle;
				igniteButtonStyle.Normal = IgniteButtonPressedBrush;
				igniteButtonStyle.Hovered = IgniteButtonPressedBrush;
				igniteButtonStyle.Pressed = IgniteButtonPressedBrush;
				IgniteButton->SetStyle(igniteButtonStyle);
				OwnerActor->SetIgniteState(true);
				break;
			}
		}
	}
}
