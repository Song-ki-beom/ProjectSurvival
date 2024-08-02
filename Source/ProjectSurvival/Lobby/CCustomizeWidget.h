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

private:
	UFUNCTION()
		void CreateSurvivor();

	UFUNCTION()
		void ExitGame();

private:
	class ACLobbySurvivor* LobbySurvivor;
	class ACLobbySurvivorController* SurvivorController;

	UPROPERTY(meta = (BindWidget))
		class UButton* CreateButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* ExitButton;
	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* SurvivorName;
};

