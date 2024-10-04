// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Reputation.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCBTService_Reputation : public UBTService
{
	GENERATED_BODY()
	
public:
	UCBTService_Reputation();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
