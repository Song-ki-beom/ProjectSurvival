#include "CGameStateBase.h"
#include "Widget/Inventory/CItemBase.h"
#include "Utility/CDebug.h"


//ItemData = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, TEXT("DataTable'/Game/PirateIsland/Include/Datas/Widget/Inventory/DT_Items.DT_Items'")));

ACGameStateBase::ACGameStateBase()
{
    ConstructorHelpers::FObjectFinder<UDataTable> DataTableAsset(TEXT("DataTable'/Game/PirateIsland/Include/Datas/Widget/Inventory/DT_Items.DT_Items'"));
    if (DataTableAsset.Succeeded())
    {
        ItemDataTable = DataTableAsset.Object;
    }
}

//void ACGameStateBase::AddInventoryID(FName InID)
//{
//}
//
void ACGameStateBase::PerformAddID(FName InID)
{
    FItemData* itemData = ItemDataTable->FindRow<FItemData>(InID, TEXT(""));
    if (itemData)
    {
        UCItemBase* ItemCopy = NewObject<UCItemBase>(StaticClass());
        ItemCopy->ID = InID;
        ItemCopy->Quantity = 1;
        ItemCopy->ItemType = itemData->ItemType;
        ItemCopy->TextData = itemData->TextData;
        ItemCopy->ItemStats = itemData->ItemStats;
        ItemCopy->NumericData = itemData->NumericData;
        ItemCopy->AssetData = itemData->AssetData;
        ItemCopy->bIsCopy = true;
    
        SharedInventoryObject.Add(ItemCopy);
    }
    
    for (UCItemBase* tempItem : SharedInventoryObject)
    {
        CDebug::Print("Item :", tempItem);
    }

    
    //SharedInventoryID.Add(InID);
    //
    //for (FName tempName : SharedInventoryID)
    //{
    //    CDebug::Print("TempName : ", tempName, FColor::Magenta);
    //
    //
    //}
}

void ACGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    //DOREPLIFETIME(ACGameStateBase, SharedInventoryID);
    DOREPLIFETIME(ACGameStateBase, SharedInventoryObject);
}