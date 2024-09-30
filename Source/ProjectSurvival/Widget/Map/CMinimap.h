#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CMinimap.generated.h"

UCLASS()
class PROJECTSURVIVAL_API UCMinimap : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UCMinimap(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	FVector2D GetCharacterPosOnWorldMap();

public:
	UFUNCTION(BlueprintCallable)
		void SetUpMinimap();

private:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UCMinimap> MinimapClass;

	FSlateBrush MinimapBrush;

	UPROPERTY(meta = (BindWidget))
		class UImage* WorldMapImage;
	UPROPERTY(meta = (BindWidget))
		class UImage* PlayerLocation;

	float WorldMapLevelWidth = 41859.0f;
	float WorldMapLevelHeight = 39879.0f;
	FVector2D WorldMapLevelTopLeftLocation = FVector2D(15700.0f, -31483.0f);
	FVector2D ImageSize = FVector2D(972.4f, 1021.5f);

};
