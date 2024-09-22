// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/DamageInterface.h"
#include "CEnemy.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACEnemy : public ACharacter, public IDamageInterface
{
	GENERATED_BODY()

public:
	ACEnemy();

protected:
	virtual void BeginPlay() override;


public:	
	virtual void Tick(float DeltaTime) override;


	virtual void DoAction();

	UFUNCTION(NetMulticast, Reliable)
	virtual void BroadcastDoAction(int32 InAttackIdx);
	UFUNCTION(Server, Reliable)
	virtual void RequestDoAction();
	virtual void PerformDoAction(int32 InAttackIdx);
	virtual void AttackTraceHit();
	virtual void Begin_DoAction();
	virtual void End_DoAction();
	


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
//Damage Interface Override
	virtual void Damage(ACharacter* Attacker, class AActor* Causer, FHitData HitData) override;



public:
	
protected: // 하위 클래스에서 설정하고 동적 로딩하기 위해 Protected 설정
	//Mesh
	FString SkeletalMeshPath;
	FString AnimInstancePath; 
	FString BBAssetPath;

	//DoAction
	UPROPERTY(EditAnywhere)
	TArray<FDoActionData> DoActionDatas;

private:
	//AI 
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;
	UPROPERTY(VisibleAnywhere, Category = "AI")
		class UCEnemyAIComponent* AIComponent;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		uint8 TeamID = 2; // Enemy 가 속한 TeamID
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class ACEnemyAIController* AIController;
	//Component
	UPROPERTY(VisibleAnywhere)
	class UCStatusComponent* StatusComponent;
	UPROPERTY(VisibleAnywhere)
	class UCMovingComponent* MovingComponent;
	UPROPERTY(VisibleAnywhere)
	class UCStateComponent* StateComponent;
	UPROPERTY(VisibleAnywhere)
	class UCMontageComponent* MontageComponent;


	//Attack
	float TraceDistance = 45.0f;
	float TraceOffset = 200.0f;
	int32 AttackIdx = 0;
public:
//ForceInline Getter & Settter
FORCEINLINE uint8 GetTeamID() { return TeamID; }

FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }

//FORCEINLINE class ACPatrolPath* GetPatrolPath() { return PatrolPath; }


	
};
