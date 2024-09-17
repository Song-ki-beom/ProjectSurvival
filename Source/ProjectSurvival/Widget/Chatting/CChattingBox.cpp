#include "Widget/Chatting/CChattingBox.h"
#include "Widget/Chatting/CChattingMessage.h"
#include "Widget/Menu/CInventoryMenu.h"
#include "Widget/CMainHUD.h"
#include "Components/VerticalBox.h"
#include "Components/Overlay.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "CGameInstance.h"
#include "Character/CSurvivorController.h"
#include "Character/CSurvivor.h"
#include "Utility/CDebug.h"

FReply UCChattingBox::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	if (InKeyEvent.GetKey() == EKeys::Enter)
	{
		InputMessageBox->SetFocus();
	
		return FReply::Handled();
	}

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

	SurvivorNameColor = FLinearColor::White;

	return true;
}

void UCChattingBox::SetInputMode()
{
	InputMessageBox->SetFocus();
}

void UCChattingBox::ClickSendButton()
{
	//if (!InputMessageBox->GetText().IsEmpty())
	//{
	//	if (this->GetOwningPlayer()->HasAuthority())
	//		SurvivorController->BroadcastMessage(InputMessageBox->GetText());
	//	else
	//		SurvivorController->RequestMessage(InputMessageBox->GetText());
	//}
	//	//AddMessageToMessageBox(InputMessageBox->GetText());
	//
	//InputMessageBox->SetText(FText::GetEmpty());
}

void UCChattingBox::SendMessage(const FText& InText, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		ACSurvivor* survivor = Cast<ACSurvivor>(GetOwningPlayerPawn());
		if (!survivor)
			return;

		if (!InText.IsEmpty())
		{
			FText survivorNameText = FText::Format(FText::FromString("{0}:"), SurvivorName);
			survivor->ReceiveMessage(survivorNameText, InText);
		}

		InputMessageBox->SetText(FText::GetEmpty());
		
		ACMainHUD* mainHUD = Cast<ACMainHUD>(GetOwningPlayer()->GetHUD());
		if (!mainHUD)
			return;

		ESlateVisibility inventoryVisibility = mainHUD->GetSurvivorInventoryWidget()->GetVisibility();

		if (inventoryVisibility == ESlateVisibility::Visible)
		{
			mainHUD->GetSurvivorInventoryWidget()->SetKeyboardFocus();
			mainHUD->GetSurvivorInventoryWidget()->SetFocus();
		}
		else
			GetOwningPlayer()->SetInputMode(FInputModeGameOnly());
	}

	if (CommitMethod == ETextCommit::OnUserMovedFocus)
	{
		CDebug::Print("Focus Moved");
		InputMessageBox->SetText(InputMessageBox->GetText());
	}
}

void UCChattingBox::ChangeSurvivorNameColor(const FLinearColor& InLinearColor)
{
	SurvivorNameColor = InLinearColor;
}

void UCChattingBox::AddMessageToMessageBox(const FText& InSurvivorNameText, const FText& InMessageText)
{
	if (ChattingMessageClass)
	{
		UCChattingMessage* chattingMessage = CreateWidget<UCChattingMessage>(this, ChattingMessageClass);
		if (chattingMessage)
		{
			chattingMessage->SetSurvivorName(InSurvivorNameText, SurvivorNameColor);
			chattingMessage->SetMessage(InMessageText);
			MessageBox->AddChild(chattingMessage);

			// 메시지가 100개 이상인 경우 가장 오래된 메시지 삭제
			if (MessageBox->GetChildrenCount() > 100)
			{
				UWidget* oldestMessage = MessageBox->GetChildAt(0);
				MessageBox->RemoveChild(oldestMessage);
			}

			MessageScrollBox->ScrollToEnd();
		}
	}
}