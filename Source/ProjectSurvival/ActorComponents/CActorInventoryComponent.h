// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CActorInventoryComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSURVIVAL_API UCActorInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCActorInventoryComponent();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
		class AActor* OwnerActor;
};
