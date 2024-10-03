// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Camera/CameraShake.h"
#include "CStatusComponent.generated.h"

//UENUM(BlueprintType)
//enum class EStatusType : uint8
//{
//	Hunger,Starvation, 
//};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthUpdated, float, NewHealthRatio);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHungerUpdated, float, NewHungerRatio);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaUpdated, float, NewStaminaRatio);

DECLARE_MULTICAST_DELEGATE(FOnLowHealthDetected);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSURVIVAL_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCStatusComponent();
	//Health
	void  ApplyDamage(float InAmount);
	bool CheckHPCoefChanged();

	//Stamina
	void ReduceStamina(float ReduceAmount);
	bool CanSpendStamina(float ReduceAmount);
	void SuspendStaminaRecover();
	void ProceedStaminaRecover();

	//Hunger
	bool IsStarving();
	void RecoverHunger(float RecoverAmount);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ReduceHungerByTime();
	void RecoverStaminaByTime();
	
	UFUNCTION(NetMulticast, Reliable)
	void BroadcastUpdateHealth(float NewHealth);

	UFUNCTION(NetMulticast, Reliable)
	void BroadcastUpdateHunger(float NewHunger);

	UFUNCTION(NetMulticast, Reliable)
		void BroadcastUpdateStamina(float NewStamina);


public:
	FOnHealthUpdated OnHealthUpdated;
	FOnHungerUpdated OnHungerUpdated;
	FOnStaminaUpdated OnStaminaUpdated;
	FOnLowHealthDetected OnLowHealthDetected;
	FTimerHandle StaminaRecoverTimerHandle;
	FTimerHandle HungerReductionTimerHandle;
	FTimerHandle StarvationTimerHandle;

private:
	class ACharacter* OwnerCharacter;
	//Health
	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100.0f;
	float CurrentHealth = 200.0f;
	int32 DamagedHealthCoef = 0;
	bool CoefChanged = false;
	
	//Hunger
	UPROPERTY(EditAnywhere, Category = "Stamina")
	float MaxHunger = 100.0f;
	float CurrentHunger = 200.0f;
	UPROPERTY(EditAnywhere, Category = "Health")
		float HungerDecreaseAmount = 5.0f;

	//Stamina
	UPROPERTY(EditAnywhere, Category = "Stamina")
		float MaxStamina = 100.0f;
	float CurrentStamina = 200.0f;
	UPROPERTY(EditAnywhere, Category = "Health")
		float StaminaIncreaseAmount = 1.5f;


	UPROPERTY(EditAnywhere)
	TSubclassOf<UMatineeCameraShake> StarveCameraShakeClass; //허기로 인한 Starve 이펙트 카메라 쉐이크 
	
	float TimeSinceStarvation = 0.0f;
	//DifficultyCoef<- 난이도 
	float DifficultyCoef = 1.0f;


public:
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE void SetMaxHealth(float NewMaxHealth) {  MaxHealth = NewMaxHealth; }
	FORCEINLINE float GetMaxHunger() { return MaxHunger; }
	FORCEINLINE void SetMaxHunger(float NewMaxHunger) { MaxHunger = NewMaxHunger; }
	FORCEINLINE float GetHealth() { return CurrentHealth; }
	FORCEINLINE bool  IsDead() { return CurrentHealth <= 0.0f; }
};


		
