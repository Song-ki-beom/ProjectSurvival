#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CWaitingWidget.generated.h"

/**
* 
*/

UCLASS()
class PROJECTSURVIVAL_API UCWaitingWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UCWaitingWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool Initialize() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetUpWidget();
	void UpdateDifficultyWidget(int InIndex);

private:
	class ACLobbySurvivor* LobbySurvivor;
	class ACLobbyGameMode* LobbyGameMode;

	UPROPERTY(meta = (BindWidget))
		class UCDifficultyWidget* DifficultyWidget;

};

