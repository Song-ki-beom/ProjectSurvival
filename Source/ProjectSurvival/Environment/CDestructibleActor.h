// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#pragma warning(push)
#pragma warning(disable : 4996)
#pragma warning(disable : 4706)

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructibleMesh.h"
#include "CDestructibleActor.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACDestructibleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACDestructibleActor();


protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


public:
	void SetUp(UDestructibleMesh* InDestructibleMesh, FTransform InstanceTransform, float InMaxDamageThreshold, int32 InDropItemRatio);
	class UDestructibleComponent* GetDestructibleComponent();
	float GetAccumulatedDamage();
	UFUNCTION()
		void AccumulateDamage(float DamageAmount);

private:
	void DestroyDestructibleActor();
	UFUNCTION(NetMulticast, Reliable)
		void BroadcastAccumulateDamage(float NewAccumulatedDamage);
	UFUNCTION()
		void OnRep_AccumulateDamage();
	UFUNCTION()
		void OnRef_DestructibleMeshSet();


	
private:
	FTimerHandle TimerHandle;
	UPROPERTY(VisibleAnywhere,Replicated)
		class UDestructibleComponent* DestructibleComponent;
	UPROPERTY(VisibleAnywhere, Replicated, ReplicatedUsing = OnRef_DestructibleMeshSet)
		class UDestructibleMesh* DestructibleMesh;
	UPROPERTY(VisibleAnywhere ,Replicated, ReplicatedUsing = OnRep_AccumulateDamage)
		float AccumulatedDamage=0.0f;
	UPROPERTY(VisibleAnywhere,Replicated)
		int32 DropItemRatio = 3;
	UPROPERTY(VisibleAnywhere,Replicated)
		float MaxDamageThreshold = 0.0f;
};
