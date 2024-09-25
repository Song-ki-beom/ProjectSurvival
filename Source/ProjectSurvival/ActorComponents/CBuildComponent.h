#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Build/CStructure.h"
#include "CustomDataType/BuildStructureDataType.h"
#include "CBuildComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSURVIVAL_API UCBuildComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCBuildComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SelectQ(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement, FName InItemID);
	void SelectW(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement, FName InItemID);
	void SelectE(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement, FName InItemID);
	void SelectA(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement, FName InItemID);
	void SelectS(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement, FName InItemID);
	void SelectD(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement, FName InItemID);
	void SelectZ(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement, FName InItemID);
	void SelectX(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement, FName InItemID);
	void SelectC(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement, FName InItemID);
	void BuildSpawnedStructure();
	void ClearSpawnedStructure();
	bool CheckIsBuilding() { return bIsBuilding; }

private:
	void SpawnBuildStructureElement(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement, FName InItemID);
	void BuildStartFoundation();
	void BuildStartWall();
	void BuildStartCeiling();
	void BuildStartRamp();
	void BuildStartDoorFrame();
	void BuildStartDoor();
	void BuildStartStair();
	void BuildStartPlaceable();

	void PerformBuild(TSubclassOf<ACStructure> InClass, FTransform InTransform);
	UFUNCTION(Server, Reliable, WithValidation)
		void RequestBuild(TSubclassOf<ACStructure> InClass, FTransform InTransform);

private:
	class ACSurvivor* Survivor;

	//UPROPERTY()
	//	class UCBuildWidget* BuildWidget;
	UPROPERTY(Replicated)
		class ACStructure* SpawnedStructure;
	UPROPERTY()
		class ACStructure_Foundation* SpawnedFoundation;
	UPROPERTY()
		class ACStructure_Wall* SpawnedWall;
	UPROPERTY()
		class ACStructure_Ceiling* SpawnedCeiling;
	UPROPERTY()
		class ACStructure_Ramp* SpawnedRamp;
	UPROPERTY()
		class ACStructure_DoorFrame* SpawnedDoorFrame;
	UPROPERTY()
		class ACStructure_Door* SpawnedDoor;
	UPROPERTY()
		class ACStructure_Stair* SpawnedStair;
	UPROPERTY()
		class ACStructure_Placeable* SpawnedPlaceable;
	UPROPERTY()
		class UDataTable* ItemDataTable;

	UMaterialInstance* RedMaterial;
	UMaterialInstance* GreenMaterial;

	TSubclassOf<ACStructure> StructureClass;
	EBuildStructureElement StructureElement;
	FTransform StructureTransform;
	bool bIsBuildable;
	bool bIsSnapped;
	bool bIsBuilding;
};
