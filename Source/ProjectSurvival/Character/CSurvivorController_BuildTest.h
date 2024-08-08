#pragma once

#include "CoreMinimal.h"
#include "Character/CSurvivorController.h"
#include "CSurvivorController_BuildTest.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACSurvivorController_BuildTest : public ACSurvivorController
{
	GENERATED_BODY()
	
public:
	ACSurvivorController_BuildTest();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public:
	////////////////////////////////////////////////////
	void SetupBuildWidget();
	void ToggleBuildWidget();

private:
	UPROPERTY()
		TSubclassOf<class UUserWidget> BuildWidgetClass;
	UPROPERTY()
		class UCBuildWidget* BuildWidget;
	bool bIsBuildWidgetOn;
};
