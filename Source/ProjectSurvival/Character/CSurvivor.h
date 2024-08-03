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

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void OnMoveForward(float InAxisValue);
	void OnMoveRight(float InAxisValue);
	void OnHorizontalLook(float InAxisValue);
	void OnVerticalLook(float InAxisValue);

public:
	void SlashTree();

private:
	void PerformSetSurvivorName(const FText& InText);
	UFUNCTION(Server, Reliable, WithValidation)
		void RequestSetSurvivorName(const FText& InText);
	void UpdateSurvivorNameWidget();
	
private:
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
		TSubclassOf<class UUserWidget> SurvivorNameClass;
	UPROPERTY(VisibleAnywhere)
		class UWidgetComponent* SurvivorNameWidgetComponent;
	
private:
	UPROPERTY(EditAnywhere)
	float TraceDistance = 1000.0f;

	UPROPERTY(ReplicatedUsing = OnRep_ReplicatedSurvivorName)
		FText ReplicatedSurvivorName;
	UFUNCTION()
		void OnRep_ReplicatedSurvivorName();
};

