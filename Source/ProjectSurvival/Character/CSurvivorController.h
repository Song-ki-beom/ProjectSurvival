#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CSurvivorController.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACSurvivorController : public APlayerController
{
	GENERATED_BODY()

public:
	ACSurvivorController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void Slash();


	
};

