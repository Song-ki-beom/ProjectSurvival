#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CProduceComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSURVIVAL_API UCProduceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCProduceComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void Produce();

		
};
