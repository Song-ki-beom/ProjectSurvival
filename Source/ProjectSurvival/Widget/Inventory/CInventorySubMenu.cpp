#include "Widget/Inventory/CInventorySubMenu.h"
#include "Widget/Inventory/CInventoryItemSlot.h"
#include "Widget/Build/CBuildWidget.h"
#include "Character/CSurvivorController.h"
#include "Components/EditableText.h" 
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Utility/CDebug.h"
//#include "CustomDataType/BuildStructureDataType.h"

void UCInventorySubMenu::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);
    if (InFocusEvent.GetCause() == EFocusCause::Mouse)
    {
        if (!IsHovered())
        {
            OnFocusOnSubMenuEnded.Broadcast();
            if (SlotReference)
                SlotReference->ToggleTooltip();

           // NativeOnMouseEnter(Geometry, PointerEvent);
            return;
        }
    }
}

void UCInventorySubMenu::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{


}

void UCInventorySubMenu::NativeConstruct()
{
    BuildStructureData = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, TEXT("DataTable'/Game/PirateIsland/Include/Datas/Widget/Build/DT_BuildStructureInfo.DT_BuildStructureInfo'")));
    SurvivorController = Cast<ACSurvivorController>(this->GetOwningPlayer());
    BuildButtons.Add(Button_Q);
    BuildButtons.Add(Button_W);
    BuildButtons.Add(Button_E);
    BuildButtons.Add(Button_A);
    BuildButtons.Add(Button_S);
    BuildButtons.Add(Button_D);
    BuildButtons.Add(Button_Z);
    BuildButtons.Add(Button_X);
    BuildButtons.Add(Button_C);

    ButtonNormalBrush = Button_Q->WidgetStyle.Normal;
    ButtonPressedBrush = Button_Q->WidgetStyle.Pressed;

    //SurvivorController->SetupBuildWidget();
}

bool UCInventorySubMenu::Initialize()
{
    bool Success = Super::Initialize();
   
    if (IsValid(SplitButton))
        SplitButton->OnClicked.AddDynamic(this, &UCInventorySubMenu::HandleOnSplitButtonClicked);
    if (IsValid(FinalSplitButton))
        FinalSplitButton->OnClicked.AddDynamic(this, &UCInventorySubMenu::HandleOnFinalSplitButtonClicked);
    if (IsValid(CancelButton))
        CancelButton->OnClicked.AddDynamic(this, &UCInventorySubMenu::HandleOnCancelButtonClicked);
    if (IsValid(BuildRegisterCancelButton))
        BuildRegisterCancelButton->OnClicked.AddDynamic(this, &UCInventorySubMenu::HandleOnCancelButtonClicked);
    if (IsValid(BuildRegisterButton))
        BuildRegisterButton->OnClicked.AddDynamic(this, &UCInventorySubMenu::HandleBuildRegisterButton);
    if (IsValid(Button_Q))
        Button_Q->OnClicked.AddDynamic(this, &UCInventorySubMenu::SelectQ);
    if (IsValid(Button_W))
        Button_W->OnClicked.AddDynamic(this, &UCInventorySubMenu::SelectW);
    if (IsValid(Button_E))
        Button_E->OnClicked.AddDynamic(this, &UCInventorySubMenu::SelectE);
    if (IsValid(Button_A))
        Button_A->OnClicked.AddDynamic(this, &UCInventorySubMenu::SelectA);
    if (IsValid(Button_S))
        Button_S->OnClicked.AddDynamic(this, &UCInventorySubMenu::SelectS);
    if (IsValid(Button_D))
        Button_D->OnClicked.AddDynamic(this, &UCInventorySubMenu::SelectD);
    if (IsValid(Button_Z))
        Button_Z->OnClicked.AddDynamic(this, &UCInventorySubMenu::SelectZ);
    if (IsValid(Button_X))
        Button_X->OnClicked.AddDynamic(this, &UCInventorySubMenu::SelectX);
    if (IsValid(Button_C))
        Button_C->OnClicked.AddDynamic(this, &UCInventorySubMenu::SelectC);

    TextItemType = EItemType::NoType;

    SubMenuSizeBox->SetVisibility(ESlateVisibility::Visible);
    SplitMenuSizeBox->SetVisibility(ESlateVisibility::Collapsed);
    BuildRegisterMenuSizeBox->SetVisibility(ESlateVisibility::Collapsed);



    if (!Success) return false;
    return true;
}

void UCInventorySubMenu::UpdateSubMenu(ERightClickStartWidget InRightClickStartWidget, bool bIsStackable)
{
    SubMenuSizeBox->SetVisibility(ESlateVisibility::Visible);
    SplitMenuSizeBox->SetVisibility(ESlateVisibility::Collapsed);
    BuildRegisterMenuSizeBox->SetVisibility(ESlateVisibility::Collapsed);

    if (SlotReference)
    {
        ActionButton->OnClicked.Clear();
        TextItemType = SlotReference->GetItemReference()->ItemType;

        switch (TextItemType)
        {
        case EItemType::Harvest:
            ActionButton->SetVisibility(ESlateVisibility::Collapsed);
            if (IsValid(SplitButton))
            {
                if (bIsStackable)
                    SplitButton->SetVisibility(ESlateVisibility::Visible);
                else
                    SplitButton->SetVisibility(ESlateVisibility::Collapsed);
            }
            break;
        case EItemType::Hunt:
            if (IsValid(ActionButton))
            {
                if (InRightClickStartWidget == ERightClickStartWidget::HideActionButtonWidget)
                {
                    CDebug::Print("Collapsed ActionButton : ", FColor::Cyan);
                    ActionButton->SetVisibility(ESlateVisibility::Collapsed);
                }
                else
                {
                    ActionButton->SetVisibility(ESlateVisibility::Visible);
                    ActionButton->OnClicked.AddDynamic(this, &UCInventorySubMenu::HandleOnUseButtonClicked);
                    ActionText->SetText(SlotReference->GetItemReference()->TextData.UsageText);
                }
            }
            if (IsValid(SplitButton))
            {
                if (bIsStackable)
                    SplitButton->SetVisibility(ESlateVisibility::Visible);
                else
                    SplitButton->SetVisibility(ESlateVisibility::Collapsed);
            }
            break;

        case EItemType::Consumable:
            if (IsValid(ActionButton))
            {
                ActionButton->SetVisibility(ESlateVisibility::Collapsed);

                //if (InRightClickStartWidget == ERightClickStartWidget::HideActionButtonWidget)
                //    ActionButton->SetVisibility(ESlateVisibility::Collapsed);
                //else
                //{
                //    ActionButton->SetVisibility(ESlateVisibility::Visible);
                //    ActionButton->OnClicked.AddDynamic(this, &UCInventorySubMenu::HandleOnUseButtonClicked);
                //    ActionText->SetText(SlotReference->GetItemReference()->TextData.UsageText);
                //}
            }
            if (IsValid(SplitButton))
            {
                if (bIsStackable)
                    SplitButton->SetVisibility(ESlateVisibility::Visible);
                else
                    SplitButton->SetVisibility(ESlateVisibility::Collapsed);
            }
            break;
        case EItemType::Build:
            if (IsValid(ActionButton))
            {
                if (InRightClickStartWidget == ERightClickStartWidget::HideActionButtonWidget)
                    ActionButton->SetVisibility(ESlateVisibility::Collapsed);
                else
                {
                    ActionButton->SetVisibility(ESlateVisibility::Visible);
                    ActionButton->OnClicked.AddDynamic(this, &UCInventorySubMenu::HandleOnBuildButtonClicked);
                    ActionText->SetText(SlotReference->GetItemReference()->TextData.UsageText);
                }
            }
            if (IsValid(SplitButton))
            {
                if (bIsStackable)
                    SplitButton->SetVisibility(ESlateVisibility::Visible);
                else
                    SplitButton->SetVisibility(ESlateVisibility::Collapsed);
            }
            break;
        case EItemType::Container:
            if (IsValid(ActionButton))
            {
                if (InRightClickStartWidget == ERightClickStartWidget::HideActionButtonWidget)
                    ActionButton->SetVisibility(ESlateVisibility::Collapsed);
                else
                {
                    ActionButton->SetVisibility(ESlateVisibility::Visible);
                    ActionButton->OnClicked.AddDynamic(this, &UCInventorySubMenu::HandleOnBuildButtonClicked);
                    ActionText->SetText(SlotReference->GetItemReference()->TextData.UsageText);
                }
            }
            if (IsValid(SplitButton))
            {
                if (bIsStackable)
                    SplitButton->SetVisibility(ESlateVisibility::Visible);
                else
                    SplitButton->SetVisibility(ESlateVisibility::Collapsed);
            }
            break;
        default:
            break;
        }
    }
    else
        CDebug::Print("Slot Reference is not valid", FColor::White);

}

void UCInventorySubMenu::HandleOnUseButtonClicked()
{
   // OnUseButtonClicked.Broadcast();
    OnFocusOnSubMenuEnded.Broadcast();
    SlotReference->ToggleTooltip();
    //Item Use 구현 부분 
}

void UCInventorySubMenu::HandleOnBuildButtonClicked()
{
    SubMenuSizeBox->SetVisibility(ESlateVisibility::Collapsed);
    BuildRegisterMenuSizeBox->SetVisibility(ESlateVisibility::Visible);
    //OnFocusOnSubMenuEnded.Broadcast();
    //SlotReference->ToggleTooltip();
    //Build 등록 구현 부분 
}


void UCInventorySubMenu::HandleOnSplitButtonClicked()
{
    //Split 구현 부분 
    SubMenuSizeBox->SetVisibility(ESlateVisibility::Collapsed);
    SplitMenuSizeBox->SetVisibility(ESlateVisibility::Visible);
}

void UCInventorySubMenu::HandleOnFinalSplitButtonClicked()
{

    FText InputNumText = SplitQuantityEditText->GetText();
    if (!InputNumText.IsEmpty())
    {

        // FText 를 FString으로 변환
        FString InputNumString = InputNumText.ToString();
        // IsNumeric -> 숫자인지 확인하는 bool Return 함수 
        bool IsNumeric = InputNumString.IsNumeric();

        if (IsNumeric)
        {
            // 숫자로 변환
            int32 InputNum = FCString::Atoi(*InputNumString);
            if (SlotReference->Split(InputNum))
            {
               /* OnFocusOnSubMenuEnded.Broadcast();
                SlotReference->ToggleTooltip();*/
            }
            else
            {
                CDebug::Print("The Split failed Due to Quantity or Slot issue or CItemBaseReference Issue");
            }
            OnFocusOnSubMenuEnded.Broadcast();
            SlotReference->ToggleTooltip();
        }
        else
        {
            CDebug::Print("The Split Input Text is not composed with only Number");
        }
    }


    //슬롯에서 스플릿 구현 
}

void UCInventorySubMenu::HandleOnCancelButtonClicked()
{
    if (SelectedStructureKey != ESelectedStructure::None)
        SelectedStructureKey = ESelectedStructure::None;

    SetButtonStyle(nullptr);

    OnFocusOnSubMenuEnded.Broadcast();
     SlotReference->ToggleTooltip();
}

void UCInventorySubMenu::HandleBuildRegisterButton()
{
    if (SelectedStructureKey == ESelectedStructure::None)
        return;

    FName slotID = SlotReference->GetItemReference()->ID;

    FBuildStructureInfo* structureInfo = BuildStructureData->FindRow<FBuildStructureInfo>(slotID, TEXT(""));
    UTexture2D* texture = structureInfo->StructureTexture;
    TSubclassOf<ACStructure> structureClass = structureInfo->StructureClass;
    EBuildStructureElement structureElem = structureInfo->StructureElement;

    if (SurvivorController->GetBuildWidget())
    {
        SurvivorController->GetBuildWidget()->SaveStructureInfo(slotID, SelectedStructureKey, texture, structureClass, structureElem);
        SelectedStructureKey = ESelectedStructure::None;
        SetButtonStyle(nullptr);
        OnFocusOnSubMenuEnded.Broadcast();
        SlotReference->ToggleTooltip();
    }
    else
        CDebug::Print("BuildWidget is not Valid");
}

void UCInventorySubMenu::SelectQ()
{
    if (SelectedStructureKey == ESelectedStructure::Q)
        return;

    SetButtonStyle(Button_Q);
    SelectedStructureKey = ESelectedStructure::Q;
}

void UCInventorySubMenu::SelectW()
{
    if (SelectedStructureKey == ESelectedStructure::W)
        return;

    SetButtonStyle(Button_W);
    SelectedStructureKey = ESelectedStructure::W;
}

void UCInventorySubMenu::SelectE()
{
    if (SelectedStructureKey == ESelectedStructure::E)
        return;

    SetButtonStyle(Button_E);
    SelectedStructureKey = ESelectedStructure::E;
}

void UCInventorySubMenu::SelectA()
{
    if (SelectedStructureKey == ESelectedStructure::A)
        return;

    SetButtonStyle(Button_A);
    SelectedStructureKey = ESelectedStructure::A;
}

void UCInventorySubMenu::SelectS()
{
    if (SelectedStructureKey == ESelectedStructure::S)
        return;

    SetButtonStyle(Button_S);
    SelectedStructureKey = ESelectedStructure::S;
}

void UCInventorySubMenu::SelectD()
{
    if (SelectedStructureKey == ESelectedStructure::D)
        return;

    SetButtonStyle(Button_D);
    SelectedStructureKey = ESelectedStructure::D;
}

void UCInventorySubMenu::SelectZ()
{
    if (SelectedStructureKey == ESelectedStructure::Z)
        return;

    SetButtonStyle(Button_Z);
    SelectedStructureKey = ESelectedStructure::Z;
}

void UCInventorySubMenu::SelectX()
{
    if (SelectedStructureKey == ESelectedStructure::X)
        return;

    SetButtonStyle(Button_X);
    SelectedStructureKey = ESelectedStructure::X;
}

void UCInventorySubMenu::SelectC()
{
    if (SelectedStructureKey == ESelectedStructure::C)
        return;

    SetButtonStyle(Button_C);
    SelectedStructureKey = ESelectedStructure::C;
}

void UCInventorySubMenu::SetButtonStyle(class UButton* InSelectedButton)
{
    for (UButton* button : BuildButtons)
    {
        if (IsValid(button))
        {
            FButtonStyle style = button->WidgetStyle;
            if (style.Normal == ButtonNormalBrush)
                continue;

            style.Normal = ButtonNormalBrush;
            button->SetStyle(style);
        }
    }

    if (IsValid(InSelectedButton))
    {
        FButtonStyle selectedStyle = InSelectedButton->WidgetStyle;
        selectedStyle.Normal = ButtonPressedBrush;
        InSelectedButton->SetStyle(selectedStyle);
    }
}


//void ACSurvivorController::TestP()
//{
//    // 빌드 위젯 이미지변경하는 임시함수
//    // 우클릭해서 등록할때 구조물 이름 및 저장위치 정보 가져와야함
//
//    FBuildStructureInfo* structureInfo = BuildStructureData->FindRow<FBuildStructureInfo>("WoodFoundation", TEXT("WoodFoundation"));
//    UTexture2D* texture = structureInfo->StructureTexture;
//    TSubclassOf<ACStructure> structureClass = structureInfo->StructureClass;
//    EBuildStructureElement structureElem = structureInfo->StructureElement;
//    BuildWidget->SaveStructureInfo(ESelectedStructure::Q, texture, structureClass, structureElem);
//
//    FBuildStructureInfo* structureInfo2 = BuildStructureData->FindRow<FBuildStructureInfo>("WoodWall", TEXT("WoodWall"));
//    UTexture2D* texture2 = structureInfo2->StructureTexture;
//    TSubclassOf<ACStructure> structureClass2 = structureInfo2->StructureClass;
//    EBuildStructureElement structureElem2 = structureInfo2->StructureElement;
//    BuildWidget->SaveStructureInfo(ESelectedStructure::W, texture2, structureClass2, structureElem2);
//
//    FBuildStructureInfo* structureInfo3 = BuildStructureData->FindRow<FBuildStructureInfo>("WoodCeiling", TEXT("WoodCeiling"));
//    UTexture2D* texture3 = structureInfo3->StructureTexture;
//    TSubclassOf<ACStructure> structureClass3 = structureInfo3->StructureClass;
//    EBuildStructureElement structureElem3 = structureInfo3->StructureElement;
//    BuildWidget->SaveStructureInfo(ESelectedStructure::E, texture3, structureClass3, structureElem3);
//
//    FBuildStructureInfo* structureInfo4 = BuildStructureData->FindRow<FBuildStructureInfo>("WoodRamp", TEXT("WoodRamp"));
//    UTexture2D* texture4 = structureInfo4->StructureTexture;
//    TSubclassOf<ACStructure> structureClass4 = structureInfo4->StructureClass;
//    EBuildStructureElement structureElem4 = structureInfo4->StructureElement;
//    BuildWidget->SaveStructureInfo(ESelectedStructure::A, texture4, structureClass4, structureElem4);
//
//    FBuildStructureInfo* structureInfo5 = BuildStructureData->FindRow<FBuildStructureInfo>("WoodDoorFrame", TEXT("WoodDoorFrame"));
//    UTexture2D* texture5 = structureInfo5->StructureTexture;
//    TSubclassOf<ACStructure> structureClass5 = structureInfo5->StructureClass;
//    EBuildStructureElement structureElem5 = structureInfo5->StructureElement;
//    BuildWidget->SaveStructureInfo(ESelectedStructure::S, texture5, structureClass5, structureElem5);
//
//    FBuildStructureInfo* structureInfo6 = BuildStructureData->FindRow<FBuildStructureInfo>("WoodDoor", TEXT("WoodDoor"));
//    UTexture2D* texture6 = structureInfo6->StructureTexture;
//    TSubclassOf<ACStructure> structureClass6 = structureInfo6->StructureClass;
//    EBuildStructureElement structureElem6 = structureInfo6->StructureElement;
//    BuildWidget->SaveStructureInfo(ESelectedStructure::D, texture6, structureClass6, structureElem6);
//
//    FBuildStructureInfo* structureInfo7 = BuildStructureData->FindRow<FBuildStructureInfo>("WoodStair", TEXT("WoodStair"));
//    UTexture2D* texture7 = structureInfo7->StructureTexture;
//    TSubclassOf<ACStructure> structureClass7 = structureInfo7->StructureClass;
//    EBuildStructureElement structureElem7 = structureInfo7->StructureElement;
//    BuildWidget->SaveStructureInfo(ESelectedStructure::Z, texture7, structureClass7, structureElem7);
//
//    FBuildStructureInfo* structureInfo8 = BuildStructureData->FindRow<FBuildStructureInfo>("WorkingBench", TEXT("WorkingBench"));
//    UTexture2D* texture8 = structureInfo8->StructureTexture;
//    TSubclassOf<ACStructure> structureClass8 = structureInfo8->StructureClass;
//    EBuildStructureElement structureElem8 = structureInfo8->StructureElement;
//    BuildWidget->SaveStructureInfo(ESelectedStructure::X, texture8, structureClass8, structureElem8);
//}