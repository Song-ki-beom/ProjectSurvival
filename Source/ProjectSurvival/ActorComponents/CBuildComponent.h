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
	void SelectQ(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement);
	void SelectW(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement);
	void SelectE(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement);
	void SelectA(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement);
	void SelectS(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement);
	void SelectD(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement);
	void SelectZ(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement);
	void SelectX(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement);
	void SelectC(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement);
	void BuildSpawnedStructure();
	void ClearSpawnedStructure();
	bool CheckIsBuilding() { return bIsBuilding; }

private:
	void SpawnBuildStructureElement(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement);
	void BuildStartFoundation();
	void BuildStartWall();
	void BuildStartCeiling();
	void DestroyChildComponent(ACStructure* InStructure, EBuildStructureElement InElement);

private:
	class ACSurvivor* Survivor;

	UPROPERTY()
		class UCBuildWidget* BuildWidget;
	UPROPERTY()
		class ACStructure* SpawnedStructure;
	UPROPERTY()
		class ACStructure_Foundation* SpawnedFoundation;
	UPROPERTY()
		class ACStructure_Wall* SpawnedWall;
	UPROPERTY()
		class ACStructure_Ceiling* SpawnedCeiling;

	UMaterialInstance* RedMaterial;
	UMaterialInstance* GreenMaterial;

	EBuildStructureElement StructureElement;
	bool bIsBuildable;
	bool bIsSnapped;
	bool bIsBuilding;
};
