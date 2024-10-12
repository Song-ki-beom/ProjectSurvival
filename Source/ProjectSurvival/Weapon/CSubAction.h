// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Struct/CWeaponStructures.h"
#include "CSubAction.generated.h"


UCLASS(Blueprintable)
class PROJECTSURVIVAL_API UCSubAction : public UObject
{
		GENERATED_BODY()

public:
	UCSubAction();
public:  
	virtual void  BeginPlay
	(
		class ACharacter* InOwner,
		class ACAttachment* InAttachment,
		class UCEquipment* InEquipment,
		class UCDoAction* InDoAction
	);

public: 
	virtual void Pressed();   
	virtual void Released();
	

public:
	UFUNCTION(BlueprintNativeEvent)
		void Begin_SubAction();
	virtual void Begin_SubAction_Implementation() {};

	UFUNCTION(BlueprintNativeEvent)
		void End_SubAction();
	virtual void End_SubAction_Implementation() {};

	UFUNCTION(BlueprintNativeEvent)
		void Tick(float InDeltaTime);
	virtual void Tick_Implementation(float InDeltaTime) {};


//private:
//	UFUNCTION(NetMulticast, Reliable)
//		void BroadcastPressed();
//	UFUNCTION(Server, Reliable)
//		void RequestPressed();
//	UFUNCTION(NetMulticast, Reliable)
//		void BroadcastReleased();
//	UFUNCTION(Server, Reliable)
//		void RequestReleased();


protected: 
	UPROPERTY()  
		class ACharacter* Owner;
	UPROPERTY()
		class ACAttachment* Attachment;
	UPROPERTY()
		class UCEquipment* Equipment;
	UPROPERTY()
		class UCDoAction* DoAction;
	UPROPERTY()
		class UCMovingComponent* Movement;
	UPROPERTY()
		class UCStateComponent* State;
	

protected:
	UPROPERTY()
	bool bInAction = false;
public:
	FORCEINLINE bool GetInAction() { return bInAction; }


};
