#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CReplicateComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSURVIVAL_API UCReplicateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCReplicateComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
////////////////////////////////////////////////////////////////////////////


};
