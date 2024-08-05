#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CCustomizeWidget.generated.h"

/**
* CreateSurvivor() - Survivor생성, 머리 위 위젯설정(리플리케이트 함수 실행), CWaitingWidget 화면에 출력
* ExitGame() - 콘솔 커맨드로 게임 종료, UFUNCTION() 사용
*/

UCLASS()
class PROJECTSURVIVAL_API UCCustomizeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UCCustomizeWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool Initialize() override;

public:
	void SetUpWidget();
	void TearDownWidget();
	void GetCustomizeInfo();

private:
	UFUNCTION()
		void SubHairIndex();
	UFUNCTION()
		void AddHairIndex();
	UFUNCTION()
		void SetHairIndexText();

	UFUNCTION()
		void SubHairColorIndex();
	UFUNCTION()
		void AddHairColorIndex();
	UFUNCTION()
		void SetHairColorIndexText();

	UFUNCTION()
		void SubPantsIndex();
	UFUNCTION()
		void AddPantsIndex();
	UFUNCTION()
		void SetPantsIndexText();

	UFUNCTION()
		void SubPantsColorIndex();
	UFUNCTION()
		void AddPantsColorIndex();
	UFUNCTION()
		void SetPantsColorIndexText();

	UFUNCTION()
		void SubBootsIndex();
	UFUNCTION()
		void AddBootsIndex();
	UFUNCTION()
		void SetBootsIndexText();

	UFUNCTION()
		void SubSkinColorIndex();
	UFUNCTION()
		void AddSkinColorIndex();
	UFUNCTION()
		void SetSkinColorIndexText();

	UFUNCTION()
		void CreateSurvivor();
	UFUNCTION()
		void ExitGame();

private:
	class ACLobbySurvivor* LobbySurvivor;
	class ACLobbySurvivorController* SurvivorController;

	UPROPERTY(meta = (BindWidget))
		class UButton* HairLeftButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* HairRightButton;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* HairIndexText;
	int32 HairIndex;
	int32 MaxHairIndex;

	UPROPERTY(meta = (BindWidget))
		class UButton* HairColorLeftButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* HairColorRightButton;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* HairColorIndexText;
	int32 HairColorIndex;
	int32 MaxHairColorIndex;

	UPROPERTY(meta = (BindWidget))
		class UButton* PantsLeftButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* PantsRightButton;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PantsIndexText;
	int32 PantsIndex;
	int32 MaxPantsIndex;

	UPROPERTY(meta = (BindWidget))
		class UButton* PantsColorLeftButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* PantsColorRightButton;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PantsColorIndexText;
	int32 PantsColorIndex;
	int32 MaxPantsColorIndex;

	UPROPERTY(meta = (BindWidget))
		class UButton* BootsLeftButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* BootsRightButton;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* BootsIndexText;
	int32 BootsIndex;
	int32 MaxBootsIndex;

	UPROPERTY(meta = (BindWidget))
		class UButton* SkinColorLeftButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* SkinColorRightButton;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SkinColorIndexText;
	int32 SkinColorIndex;
	int32 MaxSkinColorIndex;

	UPROPERTY(meta = (BindWidget))
		class UButton* CreateButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* ExitButton;
	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* SurvivorName;
};

