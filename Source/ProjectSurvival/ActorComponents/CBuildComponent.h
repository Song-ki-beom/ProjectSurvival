﻿#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Build/CStructure.h"
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
	void SelectQ(TSubclassOf<ACStructure> InClass);
	void SelectW();
	void SelectE();
	void SelectA();
	void SelectS();
	void SelectD();
	void SelectZ();
	void SelectX();
	void SelectC();

private:
	class ACSurvivor_BuildTest* Survivor;

	UPROPERTY()
		class UCBuildWidget* BuildWidget;
	UPROPERTY()
		class ACStructure* SpawnedStructure;

	UMaterialInstance* RedMaterial;
	UMaterialInstance* GreenMaterial;

	bool bIsBuildable;
};
