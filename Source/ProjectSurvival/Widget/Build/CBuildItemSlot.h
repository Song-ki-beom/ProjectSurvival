#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomDataType/BuildStructureDataType.h"
#include "CBuildItemSlot.generated.h"

UCLASS()
class PROJECTSURVIVAL_API UCBuildItemSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetBuildItemSlotInfo(class UTexture2D* InTexture, FName InItemID, int32 InQuantity, TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement);
	void ClearBuildItemSlotInfo();
	void SubStructureQuantity();

	FName GetBuildItemSlotID() { return StructureID; }
	int32 GetBuildItemSlotQuantity() { return StructureQuantity; }
	TSubclassOf<ACStructure> GetBuildItemSlotClass() { return StructureClass; }
	EBuildStructureElement GetBuildItemSlotElement() { return Element; }

private:
	UPROPERTY(meta = (BindWidget))
		class UImage* StructureImage;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* StructureQuantityText;

	FName StructureID;

	int32 StructureQuantity;

	UPROPERTY()
		TSubclassOf<ACStructure> StructureClass;

	EBuildStructureElement Element;
};
