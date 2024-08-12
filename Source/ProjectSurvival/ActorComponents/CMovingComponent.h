// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CMovingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSURVIVAL_API UCMovingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCMovingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	//Move
	void OnMoveForward(float InAxisValue);
	void OnMoveRight(float InAxisValue);
	void OnHorizontalLook(float InAxisValue);
	void OnVerticalLook(float InAxisValue);

private:
	class ACSurvivor* CSurvivor;
};
