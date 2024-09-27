#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CProduceDetail.generated.h"

UCLASS()
class PROJECTSURVIVAL_API UCProduceDetail : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetProduceDetailIcon(UTexture2D* InTexture2D);
	void SetProduceDetailName(FText InText);
	void SetProduceDetailWeight(FText InText);
	void SetProduceDetailTime(FText InText);
	void SetProduceDetailMethod(FText InText);
	void SetProduceDetailFlavorText(FText InText);
	void AddResourceToProduceRecipeScroll(FName InID, UTexture2D* InTexture2D, FText InResourceName, int32 InventoryQuantity, int32 InDemandQuantity);
	void ClearRecipeScrollBox();
	void ProduceSurvivorItem(FName InID);
	void ProducePlaceableItem(FName InID, class ACStructure_Placeable* InOwner);

private:
	UPROPERTY(meta = (BindWidget))
		class UImage* ProduceDetailIcon;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ProduceDetailName;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ProduceDetailWeight;
	UPROPERTY(meta = (BindWidget))
		class URichTextBlock* ProduceDetailTime;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ProduceDetailMethod;
	UPROPERTY(meta = (BindWidget))
		class UScrollBox* ProduceDetailRecipeScroll;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ProduceDetailFlavorText;
};
