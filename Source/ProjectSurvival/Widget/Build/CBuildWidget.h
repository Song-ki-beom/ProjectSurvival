#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomDataType/BuildStructureDataType.h"
#include "CBuildWidget.generated.h"

UCLASS()
class PROJECTSURVIVAL_API UCBuildWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void SaveStructureInfo(FName InItemID, ESelectedStructure InKey, UTexture2D* InTexture, TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement);
	TSubclassOf<ACStructure> GetStructureClass(ESelectedStructure InKey);
	EBuildStructureElement GetStructureElement(ESelectedStructure InKey);
	class UCBuildItemSlot* GetBuildItemSlotQ() { return BuildItemSlotQ; }
	class UCBuildItemSlot* GetBuildItemSlotW() { return BuildItemSlotW; }
	class UCBuildItemSlot* GetBuildItemSlotE() { return BuildItemSlotE; }
	class UCBuildItemSlot* GetBuildItemSlotA() { return BuildItemSlotA; }
	class UCBuildItemSlot* GetBuildItemSlotS() { return BuildItemSlotS; }
	class UCBuildItemSlot* GetBuildItemSlotD() { return BuildItemSlotD; }
	class UCBuildItemSlot* GetBuildItemSlotZ() { return BuildItemSlotZ; }
	class UCBuildItemSlot* GetBuildItemSlotX() { return BuildItemSlotX; }
	class UCBuildItemSlot* GetBuildItemSlotC() { return BuildItemSlotC; }
	TArray<UCBuildItemSlot*> GetBuildItemSlotArray() { return BuildItemSlotArray; }
	void RefreshBuildWidgetQuantity(FName InItemID);

private:
	void RemoveSameBuildItemSlot(FName InItemID);
	int32 GetSameItemQuantity(FName InItemID);

private:
	class ACSurvivor* Survivor;

	UPROPERTY(meta = (BindWidget))
		class UCBuildItemSlot* BuildItemSlotQ;
	UPROPERTY(meta = (BindWidget))
		class UCBuildItemSlot* BuildItemSlotW;
	UPROPERTY(meta = (BindWidget))
		class UCBuildItemSlot* BuildItemSlotE;
	UPROPERTY(meta = (BindWidget))
		class UCBuildItemSlot* BuildItemSlotA;
	UPROPERTY(meta = (BindWidget))
		class UCBuildItemSlot* BuildItemSlotS;
	UPROPERTY(meta = (BindWidget))
		class UCBuildItemSlot* BuildItemSlotD;
	UPROPERTY(meta = (BindWidget))
		class UCBuildItemSlot* BuildItemSlotZ;
	UPROPERTY(meta = (BindWidget))
		class UCBuildItemSlot* BuildItemSlotX;
	UPROPERTY(meta = (BindWidget))
		class UCBuildItemSlot* BuildItemSlotC;

	TArray<UCBuildItemSlot*> BuildItemSlotArray;

	//UPROPERTY(meta = (BindWidget))
	//	class UImage* StructureImageQ;
	//UPROPERTY(meta = (BindWidget))
	//	class UImage* StructureImageW;
	//UPROPERTY(meta = (BindWidget))
	//	class UImage* StructureImageE;
	//UPROPERTY(meta = (BindWidget))
	//	class UImage* StructureImageA;
	//UPROPERTY(meta = (BindWidget))
	//	class UImage* StructureImageS;
	//UPROPERTY(meta = (BindWidget))
	//	class UImage* StructureImageD;
	//UPROPERTY(meta = (BindWidget))
	//	class UImage* StructureImageZ;
	//UPROPERTY(meta = (BindWidget))
	//	class UImage* StructureImageX;
	//UPROPERTY(meta = (BindWidget))
	//	class UImage* StructureImageC;

	//UPROPERTY(meta = (BindWidget))
	//	class UTextBlock* StructureQuantityQ;
	//UPROPERTY(meta = (BindWidget))
	//	class UTextBlock* StructureQuantityW;
	//UPROPERTY(meta = (BindWidget))
	//	class UTextBlock* StructureQuantityE;
	//UPROPERTY(meta = (BindWidget))
	//	class UTextBlock* StructureQuantityA;
	//UPROPERTY(meta = (BindWidget))
	//	class UTextBlock* StructureQuantityS;
	//UPROPERTY(meta = (BindWidget))
	//	class UTextBlock* StructureQuantityD;
	//UPROPERTY(meta = (BindWidget))
	//	class UTextBlock* StructureQuantityZ;
	//UPROPERTY(meta = (BindWidget))
	//	class UTextBlock* StructureQuantityX;
	//UPROPERTY(meta = (BindWidget))
	//	class UTextBlock* StructureQuantityC;

	//TArray<FStructureInfo> StructureInfo_Array;

	//FName StructureID_Q;
	//FName StructureID_W;
	//FName StructureID_E;
	//FName StructureID_A;
	//FName StructureID_S;
	//FName StructureID_D;
	//FName StructureID_Z;
	//FName StructureID_X;
	//FName StructureID_C;
	//
	//int32 StructureQuantity_Q;
	//int32 StructureQuantity_W;
	//int32 StructureQuantity_E;
	//int32 StructureQuantity_A;
	//int32 StructureQuantity_S;
	//int32 StructureQuantity_D;
	//int32 StructureQuantity_Z;
	//int32 StructureQuantity_X;
	//int32 StructureQuantity_C;
	//
	//TSubclassOf<ACStructure> StructureQ;
	//TSubclassOf<ACStructure> StructureW;
	//TSubclassOf<ACStructure> StructureE;
	//TSubclassOf<ACStructure> StructureA;
	//TSubclassOf<ACStructure> StructureS;
	//TSubclassOf<ACStructure> StructureD;
	//TSubclassOf<ACStructure> StructureZ;
	//TSubclassOf<ACStructure> StructureX;
	//TSubclassOf<ACStructure> StructureC;
	//
	//EBuildStructureElement ElementQ;
	//EBuildStructureElement ElementW;
	//EBuildStructureElement ElementE;
	//EBuildStructureElement ElementA;
	//EBuildStructureElement ElementS;
	//EBuildStructureElement ElementD;
	//EBuildStructureElement ElementZ;
	//EBuildStructureElement ElementX;
	//EBuildStructureElement ElementC;
};
