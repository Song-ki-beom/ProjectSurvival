// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "CSurvivor.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACSurvivor : public ACharacter 
{
	GENERATED_BODY()

public:
	ACSurvivor();

public:
	void SlashTree();
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	void OnMoveForward(float InAxisValue);
	void OnMoveRight(float InAxisValue);
	void OnHorizontalLook(float InAxisValue);
	void OnVerticalLook(float InAxisValue);
	
private:
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;
	
private:
	UPROPERTY(EditAnywhere)
	float TraceDistance = 1000.0f;
};

