// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "DestructibleComponent.h"
#include "ActorComponents/CWeaponComponent.h"
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
	void ApplyHarvestEvent(FHitResult HitResult, float InDamageAmount, EWeaponType CauserWeaponType);

private:
	bool CheckIsFoliageInstance(const FHitResult& Hit);
	void RemoveFoliageInstance(UInstancedStaticMeshComponent* InstanceToRemove, int32 InInstanceIndex);
	bool CheckIsDestructInstance(const FHitResult& Hit);
	void SwitchFoligeToDestructible(const FString& hitIndex, float damageAmount, FTransform InSpawnTransform);
	void AddForceToDestructible(float IndamageAmount, class ACDestructibleActor* InDestructibleActor);
	UFUNCTION(NetMulticast, Reliable)
		void BroadCastRemoveFoliageInstance(UInstancedStaticMeshComponent* InInstanceToRemove, int32 InInstanceIndex);
	UFUNCTION(Server, Reliable)
		void RequestAddForceToDestructible(float IndamageAmount, class ACDestructibleActor* InDestructibleActor);
	UFUNCTION(NetMulticast, Reliable)
		void BroadcastSwitchFoligeToDestructible(UDestructibleMesh* InDestructibleMesh, FTransform InstanceTransform, float InMaxDamageThreshold, int32 InDropItemRatio, float InDamageAmount);
	UFUNCTION(NetMulticast, Reliable)
		void BroadcastPlayHarvestEffect(const FHitResult HitResult);


private:
	class UCGameInstance* GameInstance;
	UPROPERTY(VisibleAnywhere, Replicated)
		FString HitIndex;
	UPROPERTY(VisibleAnywhere, Replicated)
		float DamageAmount;
	UPROPERTY(VisibleAnywhere, Replicated)
		FTransform SpawnTransform;
	UPROPERTY(VisibleAnyWhere, Replicated)
		int32 InstanceIndex = NO_INDEX;
	UPROPERTY(VisibleAnywhere, Replicated)
		class  UInstancedStaticMeshComponent* InstanceToRemove;
	UPROPERTY()
		class ACharacter* OwnerCharacter;
	UPROPERTY()
		class ACDestructibleActor* DestructibleActor;
	UPROPERTY(EditAnywhere)
		class UParticleSystem* HarvestParticle;

};
