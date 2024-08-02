#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CCustomizeWidget.generated.h"

/**
* CreateSurvivor() - Survivor����, �Ӹ� �� ��������(���ø�����Ʈ �Լ� ����), CWaitingWidget ȭ�鿡 ���
* ExitGame() - �ܼ� Ŀ�ǵ�� ���� ����, UFUNCTION() ���
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
