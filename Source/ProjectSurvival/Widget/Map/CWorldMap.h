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
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void SetCharacterPosOnWorldMap();

public:
	void SetSurvivorNameOnWorldMap(const FText& InText, uint32 NetGUIDValue);
	void SetOtherCharacterPosOnWorldMap(float LocationX, float LocationY, float RotationZ, uint32 NetGUIDValue);

private:
	UPROPERTY(meta = (BindWidget))
		class UCanvasPanel* WorldMapCanvasPanel;
	UPROPERTY(meta = (BindWidget))
		class UImage* WorldMap;
	//UPROPERTY(meta = (BindWidget))
	//	class UImage* PlayerLocation;

	float WorldMapLevelWidth = 41859.0f;
	float WorldMapLevelHeight = 39879.0f;
	FVector2D WorldMapLevelTopLeftLocation = FVector2D(15700.0f, -31483.0f);
	FVector2D ImageSize;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UCPlayerLocation> PlayerLocationClass;

	UPROPERTY()
		TMap<uint32, TWeakObjectPtr<UCPlayerLocation>> PlayerLocationMap;

	int32 PersonalNetGuidValue;

	bool bIsNameTransmitted = false;
};
