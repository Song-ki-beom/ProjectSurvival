#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CChattingBox.generated.h"

UCLASS()
class PROJECTSURVIVAL_API UCChattingBox : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual bool Initialize() override;

public:
	void SetInputMode();

private:
	void ClickSendButton();
	UFUNCTION()
		void SendMessage(const FText& InText, ETextCommit::Type CommitMethod);

	void AddMessageToMessageBox(const FText& InText);

private:
	UPROPERTY(meta = (BindWidget))
		class UVerticalBox* MessageBox;
	UPROPERTY(meta = (BindWidget))
		class UOverlay* InputOverlay;
	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* InputMessageBox;
	UPROPERTY(meta = (BindWidget))
		class UButton* SendButton;
	
	FText SurvivorName;
};
