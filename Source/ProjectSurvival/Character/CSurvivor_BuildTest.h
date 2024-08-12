#pragma once

#include "CoreMinimal.h"
#include "Character/CSurvivor.h"
#include "CSurvivorController_BuildTest.h"
#include "CustomDataType/BuildStructureDataType.h"
#include "CSurvivor_BuildTest.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACSurvivor_BuildTest : public ACSurvivor
{
	GENERATED_BODY()
	
public:
	ACSurvivor_BuildTest();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	class UCBuildComponent* GetBuildComponent() { return BuildComponent; }

	void SelectStructure(ESelectedStructure InKey, TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement);

private:
	UPROPERTY(VisibleAnywhere)
		class UCBuildComponent* BuildComponent;
};
