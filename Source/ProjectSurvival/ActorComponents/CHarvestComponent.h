// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "DestructibleComponent.h"
#include "CHarvestComponent.generated.h"


#define NO_INDEX -1

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSURVIVAL_API UCHarvestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCHarvestComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void HarvestBoxTrace(float DamageAmount);

private:
	bool CheckIsFoliageInstance(const FHitResult& Hit);
	bool CheckIsDestructInstance(const FHitResult& Hit);
	void SwitchFoligeToDestructible(FString* hitIndex, float damageAmount);
	void AddForceToDestructible(float damageAmount);

private:
	float TraceDistance = 45.0f;
	float TraceOffset = 100.0f;
	FTransform SpawnTransform;
	int32 InstanceIndex = NO_INDEX;

	class UCGameInstance* GameInstance;
	class ACharacter* OwnerCharacter;
	class ACDestructibleActor* DestructibleActor;
	

};
