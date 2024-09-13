#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CChattingMessage.generated.h"

UCLASS()
class PROJECTSURVIVAL_API UCChattingMessage : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetSurvivorName(const FText& InText, FLinearColor InLinearColor);
	void SetMessage(const FText& InText);

private:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SurvivorName;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Message;
};
