#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CWaitingWidget.generated.h"

UCLASS()
class PROJECTSURVIVAL_API UCWaitingWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UCWaitingWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool Initialize() override;
public:
	void SetUpWidget();
	void TearDownWidget();
private:
	class ACLobbySurvivor* LobbySurvivor;

	UPROPERTY(meta = (BindWidget))
		class UCDifficultyWidget* DifficultyWidget;

};

