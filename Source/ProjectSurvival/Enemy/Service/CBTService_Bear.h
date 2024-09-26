// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Bear.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCBTService_Bear : public UBTService
{
	GENERATED_BODY()
public:
	UCBTService_Bear();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "Action")
		float ActionRange = 200.0f;  

};
