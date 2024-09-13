#include "Widget/Chatting/CChattingMessage.h"
#include "Components/TextBlock.h"

void UCChattingMessage::SetSurvivorName(const FText& InText, FLinearColor InLinearColor)
{
	SurvivorName->SetText(InText);
	SurvivorName->SetColorAndOpacity(FSlateColor(InLinearColor));
}

void UCChattingMessage::SetMessage(const FText& InText)
{
	Message->SetText(InText);
}

