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
	//UFUNCTION(Server, Reliable) //클라이언트 -> 서버  
	UFUNCTION()
		void OnServer_AccumulateDamage(float NewAccumulatedDamage);
	//UFUNCTION(NetMulticast, Reliable) // 서버와 클라이언트 모두 
	UFUNCTION()
		void OnRep_AccumulateDamage(float NewAccumulatedDamage);
	

private:
	void DestroyDestructibleActor();

private:
	FTimerHandle TimerHandle;

	
	
private:
	UPROPERTY(VisibleAnywhere,Replicated)
		class UDestructibleComponent* DestructibleComponent;
	UPROPERTY(VisibleAnywhere ,Replicated, ReplicatedUsing = OnRep_AccumulateDamage)
		float AccumulatedDamage=0.0f;
	UPROPERTY(VisibleAnywhere)
		int32 DropItemRatio = 3;
	UPROPERTY(VisibleAnywhere)
		float MaxDamageThreshold = 0.0f;
};
