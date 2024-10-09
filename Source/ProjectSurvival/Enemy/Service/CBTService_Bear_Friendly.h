// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Bear_Friendly.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCBTService_Bear_Friendly : public UBTService
{
	GENERATED_BODY()
public:
	UCBTService_Bear_Friendly();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "Action")
		float ActionRange = 250.0f;

};
