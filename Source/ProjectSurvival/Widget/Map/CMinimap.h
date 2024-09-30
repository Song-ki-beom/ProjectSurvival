#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CMiniMap.generated.h"

UCLASS()
class PROJECTSURVIVAL_API UCMiniMap : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	FVector2D GetCharacterPosOnWorldMap();

private:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UCMiniMap> MiniMapClass;

	FSlateBrush MiniMapBrush;

	UPROPERTY(meta = (BindWidget))
		class UImage* WorldMap;
	UPROPERTY(meta = (BindWidget))
		class UImage* PlayerLocation;

	float WorldMapLevelWidth = 41859.0f;
	float WorldMapLevelHeight = 39879.0f;
	FVector2D WorldMapLevelTopLeftLocation = FVector2D(15700.0f, -31483.0f);
	FVector2D ImageSize;
};
