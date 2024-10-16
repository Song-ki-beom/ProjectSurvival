// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#pragma warning(push)
#pragma warning(disable : 4996)
#pragma warning(disable : 4706)

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructibleMesh.h"
#include "Struct/CDestructibleStructures.h"
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
	void SetUp(FTransform InstanceTransform, FDestructibleStruct* DestructibleStruct);
	class UDestructibleComponent* GetDestructibleComponent();
	float GetAccumulatedDamage();
	UFUNCTION()
		void AccumulateDamage(float DamageAmount);

private:
	void DestroyDestructibleActor();
	void CreateDropItem();

	UFUNCTION(NetMulticast, Reliable)
		void BroadcastAccumulateDamage(float NewAccumulatedDamage);
	UFUNCTION()
		void OnRep_AccumulateDamage();
	UFUNCTION()
		void OnRef_DestructibleMeshSet();


	
private:
	//DataTable은 처음 시작하고 게임이 Start 될 때만 초기화 된다.
	UPROPERTY(VisibleAnywhere, Category = "ItemDataTable")
		class UDataTable* ItemDataTable;

	FTimerHandle TimerHandle;
	UPROPERTY(VisibleAnywhere,Replicated)
		class UDestructibleComponent* DestructibleComponent;
<<<<<<< HEAD
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRef_DestructibleMeshSet) 
=======
	UPROPERTY(VisibleAnywhere, Replicated, ReplicatedUsing = OnRef_DestructibleMeshSet)
>>>>>>> parent of 31a491f (10.16 버그 머지)
		class UDestructibleMesh* DestructibleMesh;
	UPROPERTY(VisibleAnywhere ,Replicated, ReplicatedUsing = OnRep_AccumulateDamage)
		float AccumulatedDamage=0.0f;
	UPROPERTY(VisibleAnywhere,Replicated)
		int32 EarnItemRatio;
	UPROPERTY(VisibleAnywhere,Replicated)
		float MaxDamageThreshold;
	UPROPERTY(VisibleAnywhere, Replicated)
		int32 DropItemNum;
	UPROPERTY(VisibleAnywhere, Replicated)
		FName DropItemID;
	UPROPERTY(VisibleAnywhere, Replicated)
		float DropOffsetRange;

};
