// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthUpdated, float, NewHealthRatio);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaUpdated, float, NewStaminaRatio);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSURVIVAL_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCStatusComponent();
	void  ApplyDamage(float InAmount);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ReduceStaminaByTime();
	
	UFUNCTION(NetMulticast, Reliable)
	void BroadcastUpdateHealth(float NewHealth);

	UFUNCTION(NetMulticast, Reliable)
	void BroadcastUpdateStamina(float NewStamina);



public:
	FOnHealthUpdated OnHealthUpdated;
	FOnStaminaUpdated OnStaminaUpdated;
	FTimerHandle StaminaReductionTimerHandle;

private:
	class ACharacter* OwnerCharacter;
	//Health
	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100.0f;
	float CurrentHealth = 200.0f;
	
	//Starvation
	UPROPERTY(EditAnywhere, Category = "Stamina")
	float MaxStamina = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Health")
	float StaminaDecreaseAmount = 5.0f;
	float CurrentStamina = 200.0f;
	


public:
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE void SetMaxHealth(float NewMaxHealth) {  MaxHealth = NewMaxHealth; }
	FORCEINLINE float GetHealth() { return CurrentHealth; }
	FORCEINLINE bool  IsDead() { return CurrentHealth <= 0.0f; }
};


		
