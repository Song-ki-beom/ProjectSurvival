#include "Widget/Chatting/CChattingBox.h"
#include "Components/VerticalBox.h"
#include "Components/Overlay.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "CGameInstance.h"
#include "Utility/CDebug.h"

FReply UCChattingBox::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);
	return FReply::Unhandled();
}

bool UCChattingBox::Initialize()
{
	bool Sucess = Super::Initialize();

	if (!Sucess)
		return false;

	InputMessageBox->OnTextCommitted.AddDynamic(this, &UCChattingBox::SendMessage);

	if (!IsValid(SendButton)) { CDebug::Print("SendButton is invalid"); return false; }
	SendButton->OnClicked.AddDynamic(this, &UCChattingBox::ClickSendButton);

	UCGameInstance* gameInstance = Cast<UCGameInstance>(GetWorld()->GetGameInstance());
	if (gameInstance)
		SurvivorName = gameInstance->GetLobbySurvivorName();

	return true;
}

void UCChattingBox::SetInputMode()
{
	InputMessageBox->SetFocus();
}

void UCChattingBox::ClickSendButton()
{

}

void UCChattingBox::SendMessage(const FText& InText, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		if (!InText.IsEmpty())
			AddMessageToMessageBox(InText);

		InputMessageBox->SetText(FText::GetEmpty());
	}

	if (CommitMethod == ETextCommit::OnUserMovedFocus)
	{
		CDebug::Print("Focus Moved");
		InputMessageBox->SetText(InputMessageBox->GetText());
	}
}

void UCChattingBox::AddMessageToMessageBox(const FText& InText)
{

}