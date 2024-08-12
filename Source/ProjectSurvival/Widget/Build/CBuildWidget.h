#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomDataType/BuildStructureDataType.h"
#include "CBuildWidget.generated.h"

UCLASS()
class PROJECTSURVIVAL_API UCBuildWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SaveStructureInfo(ESelectedStructure InKey, UTexture2D* InTexture, TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement);
	TSubclassOf<ACStructure> GetStructureClass(ESelectedStructure InKey);
	EBuildStructureElement GetStructureElement(ESelectedStructure InKey);

private:
	UPROPERTY(meta = (BindWidget))
		class UImage* StructureImageQ;
	UPROPERTY(meta = (BindWidget))
		class UImage* StructureImageW;
	UPROPERTY(meta = (BindWidget))
		class UImage* StructureImageE;
	UPROPERTY(meta = (BindWidget))
		class UImage* StructureImageA;
	UPROPERTY(meta = (BindWidget))
		class UImage* StructureImageS;
	UPROPERTY(meta = (BindWidget))
		class UImage* StructureImageD;
	UPROPERTY(meta = (BindWidget))
		class UImage* StructureImageZ;
	UPROPERTY(meta = (BindWidget))
		class UImage* StructureImageX;
	UPROPERTY(meta = (BindWidget))
		class UImage* StructureImageC;

	TSubclassOf<ACStructure> StructureQ;
	TSubclassOf<ACStructure> StructureW;
	TSubclassOf<ACStructure> StructureE;
	TSubclassOf<ACStructure> StructureA;
	TSubclassOf<ACStructure> StructureS;
	TSubclassOf<ACStructure> StructureD;
	TSubclassOf<ACStructure> StructureZ;
	TSubclassOf<ACStructure> StructureX;
	TSubclassOf<ACStructure> StructureC;

	EBuildStructureElement ElementQ;
	EBuildStructureElement ElementW;
	EBuildStructureElement ElementE;
	EBuildStructureElement ElementA;
	EBuildStructureElement ElementS;
	EBuildStructureElement ElementD;
	EBuildStructureElement ElementZ;
	EBuildStructureElement ElementX;
	EBuildStructureElement ElementC;
};
