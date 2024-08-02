#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CServerRow.generated.h"

UCLASS()
class PROJECTSURVIVAL_API UCServerRow : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

public:
	void SetUpWidget(class UCLobbyWidget* InParent, uint32 InIndex);

public:
	UPROPERTY(meta = (BindWidget))
		class UButton* ServerRowButton;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ServerName;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* HostName;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Connection;
	UPROPERTY(BlueprintReadOnly)
		bool bSelected = false;

	UFUNCTION()
		void SelectServer();

private:
	UPROPERTY()
		class UCLobbyWidget* Parent;
	UPROPERTY()
		class UPanelWidget* ParentScrollBox;
	uint32 Index;
};

