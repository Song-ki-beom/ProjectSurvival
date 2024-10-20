// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Camera/CameraShake.h"
#include "CStatusComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthUpdated, float, NewHealthRatio);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHungerUpdated, float, NewHungerRatio);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaUpdated, float, NewStaminaRatio);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFriendShipUpdated, float, NewFriendShipRatio);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStarvationDetected,bool , bSetVisible);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnExhaustDetected, bool, bSetVisible);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBecameFriendly);



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
	bool IsFatalHealth();
	void RecoverHealth(float RecoverAmount);


	//Stamina
	void ReduceStamina(float ReduceAmount);
	bool CanSpendStamina(float ReduceAmount);
	void SuspendStaminaRecover();
	void ProceedStaminaRecover();
	void ReduceHungerByTime();
	void RecoverStaminaByTime();

	//Hunger
	bool IsStarving();
	void ReduceHunger(float DecreaseAmount);
	void RecoverHunger(float RecoverAmount);

	//Exhaust : 허기와 피가 둘다 없음 
	bool IsExhausted();

	//FrientShip 
	bool IsFriendly();
	void StackFriendShip(float StackAmount);

	//Respawn
	void SetRespawnStatus();


	UFUNCTION(NetMulticast, Reliable)
		void BroadcastUpdateHealth(float NewHealth);

	UFUNCTION(NetMulticast, Reliable)
		void BroadcastUpdateHunger(float NewHunger);

	UFUNCTION(NetMulticast, Reliable)
		void BroadcastUpdateStamina(float NewStamina);

	UFUNCTION(NetMulticast, Reliable)
		void BroadcastUpdateFriendShip(float NewFriendShip);

	UFUNCTION(NetMulticast, Reliable)
		void BroadcastChangeExhausted();

	UFUNCTION(NetMulticast, Reliable)
		void BroadcastCancelExhausted();

	

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
	
	

public:
	FOnHealthUpdated OnHealthUpdated;
	FOnHungerUpdated OnHungerUpdated;
	FOnStaminaUpdated OnStaminaUpdated;
	FOnFriendShipUpdated OnFriendShipUpdated;
	FOnLowHealthDetected OnLowHealthDetected;
	FOnBecameFriendly OnBecameFriendly;
	FOnStarvationDetected OnStarvationDetected;
	FOnExhaustDetected OnExhaustDetected;
	FTimerHandle StaminaRecoverTimerHandle;
	FTimerHandle HungerReductionTimerHandle;
	FTimerHandle StarvationTimerHandle;
	
	UPROPERTY()
		class ACharacter* OwnerCharacter;

protected:
	//Health
	UPROPERTY(EditAnywhere, Category = "Health")
		float MaxHealth = 100.0f;
	float CurrentHealth = 0.0f;
	int32 DamagedHealthCoef = 0;
	bool bCoefChanged = false;
	
	//Hunger
	UPROPERTY(EditAnywhere, Category = "Hunger")
		float MaxHunger = 100.0f;
	float CurrentHunger = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Hunger")
		float HungerDecreaseAmount = 3.0f;
	float TimeSinceStarvation = 0.0f;

	//Stamina
	UPROPERTY(EditAnywhere, Category = "Stamina")
		float MaxStamina = 100.0f;
	float CurrentStamina = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Stamina")
		float StaminaIncreaseAmount = 1.5f;

	//Exhausted 
	bool bIsExhausted = false;

	//Friendship Gauge
	UPROPERTY(EditAnywhere, Category = "Friendship")
		float MaxFriendShip = 100.0f;
	float CurrentFriendship = 0.0f;
	bool bIsFriendly = false;

	
	//DifficultyCoef<- 난이도 계수
	float DifficultyCoef = 1.0f;


	UPROPERTY(EditAnywhere)
	TSubclassOf<UMatineeCameraShake> StarveCameraShakeClass; //허기로 인한 Starve 이펙트 카메라 쉐이크 
	
	

	


public:
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE void SetMaxHealth(float NewMaxHealth) {  MaxHealth = NewMaxHealth; }
	FORCEINLINE float GetMaxHunger() { return MaxHunger; }
	FORCEINLINE void SetMaxHunger(float NewMaxHunger) { MaxHunger = NewMaxHunger; }
	FORCEINLINE float GetHealth() { return CurrentHealth; }
	FORCEINLINE bool  IsDead() { return CurrentHealth <= 0.0f; }
};


		
