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

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
//Damage Interface Override
	virtual void Damage(ACharacter* Attacker, class AActor* Causer, FHitData HitData) override;



public:
	
protected: // 하위 클래스에서 설정하고 동적 로딩하기 위해 Protected 설정
	//Mesh
	FString SkeletalMeshPath;
	FString AnimInstancePath; 
	FString BBAssetPath;

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
		class UCStatusComponent* StatusComponent;
		class UCMovingComponent* MovingComponent;
		class UCStateComponent* StateComponent;
		


public:
//ForceInline Getter & Settter
FORCEINLINE uint8 GetTeamID() { return TeamID; }

FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }

//FORCEINLINE class ACPatrolPath* GetPatrolPath() { return PatrolPath; }


	
};
