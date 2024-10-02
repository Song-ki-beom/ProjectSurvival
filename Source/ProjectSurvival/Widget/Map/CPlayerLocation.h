#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPlayerLocation.generated.h"

UCLASS()
class PROJECTSURVIVAL_API UCPlayerLocation : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void CheckWidgetOwner(bool InValue);
	void RegisterPlayerName(const FText& InText);
	void UpdatePlayerLocation(float LocationX, float LocationY, float RotationZ);

private:
	UPROPERTY(meta = (BindWidget))
		class UImage* PlayerLocationImage;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PlayerName;
};
