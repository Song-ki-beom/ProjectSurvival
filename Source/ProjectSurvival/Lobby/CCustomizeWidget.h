#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CCustomizeWidget.generated.h"

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
	class ACLobbySurvivor* LobbySurvivor;
	class ACLobbySurvivorController* SurvivorController;
	UPROPERTY(meta = (BindWidget))
		class UButton* CreateButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* ExitButton;
	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* SurvivorName;
	UFUNCTION()
		void CreateSurvivor();
	UFUNCTION()
		void ExitGame();
};
