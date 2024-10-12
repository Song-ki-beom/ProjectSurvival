// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Boar.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCBTService_Boar : public UBTService
{
	GENERATED_BODY()
public:
	UCBTService_Boar();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "Action")
		float ActionRange = 200.0f;
};
