// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthUpdated, float, NewHealthRatio);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSURVIVAL_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCStatusComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(NetMulticast, Reliable)
	void BroadcastUpdataHealth(float NewHealth);

public:
	FOnHealthUpdated OnHealthUpdated;


private:
	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100.0f;
	class ACharacter* OwnerCharacter;
	float CurrentHealth = 0.0f;


public:
	void  ApplyDamage(float InAmount);
public:
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE void SetMaxHealth(float NewMaxHealth) {  MaxHealth = NewMaxHealth; }
	FORCEINLINE float GetHealth() { return CurrentHealth; }
	FORCEINLINE bool  IsDead() { return CurrentHealth <= 0.0f; }
};


		
