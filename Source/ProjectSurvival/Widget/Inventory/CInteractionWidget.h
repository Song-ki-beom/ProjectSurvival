// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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

public:
	void UpdateWidget(const struct FInteractableData* InteractableData);
	void ToggleMoreInfo();
	void ShowMoreInfo();
	void HideMoreInfo();

public:
	UPROPERTY(VisibleAnywhere, Category = "Interaction widget | Player Reference")
		class ACSurvivor* PlayerReference;

protected:

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction widget | Interactable Data")
	class UTextBlock* NameText; //아이템 이름 
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction widget | Interactable Data")
	class UTextBlock* ActionText; //
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction widget | Interactable Data")
	class UTextBlock* QuantityText;

	//상호작용에 쓰이는 변수들 
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction widget | Interactable Data")
	class UProgressBar* InteractionProgressBar;
	UPROPERTY(VisibleAnywhere, Category = "Interaction widget | Interactable Data") //일시적으로 BindWidget 뺌 디버그를 위해
	float CurrentInteractionDuration;



	UPROPERTY(VisibleAnywhere,meta = (BindWidget), Category = "Interaction widget | Item More Info") //접히고 펼칠 MoreInfo Vertical Box 
		class UBorder* MoreInfoBox;


	//More Info
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


private:
		//DataTable은 처음 시작하고 게임이 Start 될 때만 초기화 된다.
		UPROPERTY(VisibleAnywhere, Category = "PickupInitialization")
		class UDataTable* ItemDataTable;
	

};
