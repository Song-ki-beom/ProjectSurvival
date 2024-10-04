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
	void RegisterPlayer(class ACSurvivor* InActor);
	void RegisterPlayerName(const FText& InText);
	void UpdatePlayerLocation(float LocationX, float LocationY, float RotationZ);
	void RemovePlayerLocation();
	void HidePlayerLocation();
	void ShowPlayerLocation();

private:
	UFUNCTION()
		void OnDestroyedSurvivor(class AActor* InActor);

private:
	UPROPERTY(meta = (BindWidget))
		class UImage* PlayerLocationImage;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PlayerName;
	UPROPERTY()
		class ACSurvivor* Survivor;
};
