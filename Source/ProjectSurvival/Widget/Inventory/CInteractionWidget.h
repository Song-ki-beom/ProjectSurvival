// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/InteractionInterface.h"
#include "CInteractionWidget.generated.h"


UCLASS()
class PROJECTSURVIVAL_API UCInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UCInteractionWidget(const FObjectInitializer& ObjectInitializer);


protected:
	//InteractionProgressBar 정보 갱신
	UFUNCTION(Category = "Interaction widget | Interactable Data")
	float UpdateInteractionProgress();

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
public:
	void UpdateWidget(const struct FInteractableData* InteractableData);
	void ToggleHiddenMenu();
	void ShowHiddenMenu();
	void HideHiddenMenu();
	void MoveFocusToNextButton();
	void MoveFocusToPrevButton();
	
private:
	UFUNCTION()
	void OnCancelButtonClicked();
	UFUNCTION()
	void OnRecallButtonClicked();
	UFUNCTION()
	void OnDestroyButtonClicked();
	UFUNCTION()
	void OnRepairButtonClicked();

public:
	UPROPERTY(VisibleAnywhere, Category = "Interaction widget | Player Reference")
		class ACSurvivor* PlayerReference;

protected:

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction widget | Interactable Data")
		class UTextBlock* NameText; //아이템 이름 
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction widget | Interactable Data")
		class UTextBlock* ActionKeyText; //상호작용 Key 표시 Text
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction widget | Interactable Data")
		class UTextBlock* ActionText; // 상호작용 이름 
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction widget | Interactable Data")
		class UTextBlock* QuantityText; //수량 
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction widget | Interactable Data")
		bool  bIsDropMesh; // 아이템이 주머니(DropMesh) 상태 인지

	//상호작용에 쓰이는 변수들 
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction widget | Interactable Data")
		class UProgressBar* InteractionProgressBar; //길게 눌렀을 때 뜨는 프로그레스 바 , 별도의 bool 설정 필요
	UPROPERTY(VisibleAnywhere, Category = "Interaction widget | Interactable Data") //일시적으로 BindWidget 뺌 디버그를 위해
		float CurrentInteractionDuration;


	//More Info
	UPROPERTY(VisibleAnywhere,meta = (BindWidget), Category = "Interaction widget | Item More Info") //접히고 펼칠 MoreInfo Vertical Box 
		class UBorder* MoreInfoBox;
	
	UPROPERTY(VisibleAnywhere,meta = (BindWidget), Category = "Interaction widget | Item More Info")
		class UTextBlock* ItemName;
	UPROPERTY(VisibleAnywhere,meta = (BindWidget), Category = "Interaction widget | Item More Info")
		class UTextBlock* ItemType;
	UPROPERTY(VisibleAnywhere,meta = (BindWidget), Category = "Interaction widget | Item More Info")
		class UTextBlock* ItemDescription;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction widget | Item More Info")
		class UTextBlock* DamageValue;
	UPROPERTY(VisibleAnywhere,meta = (BindWidget), Category = "Interaction widget | Item More Info")
		class UTextBlock* MaxStackSize;//스택
	UPROPERTY(VisibleAnywhere,meta = (BindWidget), Category = "Interaction widget | Item More Info")
		class UTextBlock* StackWeight; //무게

	//Extra Option 
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction widget | Extra Option") //접히고 펼칠 MoreInfo Vertical Box 
		class UBorder* ExtraOptionBox;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction widget | Extra Option") //접히고 펼칠 MoreInfo Vertical Box 
		class UButton* CancelButton;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction widget | Extra Option") //접히고 펼칠 MoreInfo Vertical Box 
		class UButton* RecallButton;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction widget | Extra Option") //접히고 펼칠 MoreInfo Vertical Box 
		class UButton* DestroyButton;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction widget | Extra Option") //접히고 펼칠 MoreInfo Vertical Box 
		class UButton* RepairButton;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction widget | Extra Option") //접히고 펼칠 MoreInfo Vertical Box 
		TArray<UButton*> ExtraButtonArray;


private:
		//DataTable은 처음 시작하고 게임이 Start 될 때만 초기화 된다.
		UPROPERTY(VisibleAnywhere, Category = "PickupInitialization")
		class UDataTable* ItemDataTable;
		UPROPERTY(VisibleAnywhere, Category = "Interactable Actor Type")
		EInteractableType InteractType;
		int32 ExtraButtonFoucsIndex;
		FButtonStyle DefaultButtonStyle;
};
