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
	void AddMessageToMessageBox(const FText& InSurvivorNameText, const FText& InMessageText);

private:
	UFUNCTION()
		void ClickSendButton();
	UFUNCTION()
		void SendMessage(const FText& InText, ETextCommit::Type CommitMethod);
	UFUNCTION()
		void ChangeSurvivorNameColor(const FLinearColor& InLinearColor);

private:
	UPROPERTY()
		class ACSurvivorController* SurvivorController;

	UPROPERTY(meta = (BindWidget))
		class UScrollBox* MessageScrollBox;
	UPROPERTY(meta = (BindWidget))
		class UVerticalBox* MessageBox;
	UPROPERTY(meta = (BindWidget))
		class UOverlay* InputOverlay;
	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* InputMessageBox;
	UPROPERTY(meta = (BindWidget))
		class UButton* SendButton;
	
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UCChattingMessage> ChattingMessageClass;

	FText SurvivorName;
	FLinearColor SurvivorNameColor;
};
