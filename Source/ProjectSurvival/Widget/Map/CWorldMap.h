#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget/Map/CPlayerLocation.h"
#include "CWorldMap.generated.h"

UCLASS()
class PROJECTSURVIVAL_API UCWorldMap : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	void SetCharacterPosOnWorldMap();

public:
	void CreateSurvivorLocationOnWorldMap(const FText& InText, uint32 NetGUIDValue);
	void RefreshSurvivorLocationOnWorldMap(float LocationX, float LocationY, float RotationZ, uint32 NetGUIDValue);
	FNetworkGUID GetPersonalGUID() { return PersonalNetGuid; }

private:
	void DisableNameTransmit() { bIsNameTransmitted = true; }

private:
	UPROPERTY(meta = (BindWidget))
		class UCanvasPanel* WorldMapCanvasPanel;
	UPROPERTY(meta = (BindWidget))
		class UImage* WorldMap;

	float WorldMapLevelWidth = 41859.0f;
	float WorldMapLevelHeight = 39879.0f;
	FVector2D WorldMapLevelTopLeftLocation = FVector2D(15700.0f, -31483.0f);
	FVector2D ImageSize;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UCPlayerLocation> PlayerLocationClass;

	UPROPERTY()
		TMap<uint32, TWeakObjectPtr<UCPlayerLocation>> PlayerLocationMap;

	int32 PersonalNetGuidValue;
	FNetworkGUID PersonalNetGuid;

	bool bIsNameTransmitted = false;
};
