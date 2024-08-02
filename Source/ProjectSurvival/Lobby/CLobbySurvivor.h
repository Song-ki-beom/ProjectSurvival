#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CLobbySurvivor.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACLobbySurvivor : public ACharacter
{
	GENERATED_BODY()

public:
	ACLobbySurvivor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void SetSurvivorName(const FText& InText);

	void PerformSetSurvivorName(const FText& InText);

	UFUNCTION(Server, Reliable, WithValidation)
		void RequestSetSurvivorName(const FText& InText);

	void SetLocalValue();
	void UpdateSurvivorNameWidget();

private:
	void OnMoveForward(float InAxisValue);
	void OnMoveRight(float InAxisValue);

public:
	UPROPERTY()
		FText SurvivorName;

	UPROPERTY(ReplicatedUsing = OnRep_ReplicatedSurvivorName)
		FText ReplicatedSurvivorName;

	UFUNCTION()
		void OnRep_ReplicatedSurvivorName();

private:
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

	class ACLobbySurvivorController* LobbySurvivorController;

	UPROPERTY(VisibleAnywhere)
		TSubclassOf<class UUserWidget> SurvivorNameClass;

	UPROPERTY(VisibleAnywhere)
		class UWidgetComponent* SurvivorNameWidgetComponent;

	void UpdateWidgetVisibility();

	UPROPERTY()
		float HideDistance = 3000.0f;

	FVector OriginalSpringArmLocation;
};