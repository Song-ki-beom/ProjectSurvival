// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ActorComponents/CStateComponent.h"
#include "Struct/CWeaponStructures.h"
#include "CEnemy.generated.h"



UCLASS()
class PROJECTSURVIVAL_API ACEnemy : public ACharacter
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

	virtual void DoEncounter();
	virtual void PerformDoSpecialAction(ESpecialState SpecialState);
	UFUNCTION(NetMulticast, Reliable)
	virtual void BroadcastDoSpecialAction(ESpecialState SpecialState);
	UFUNCTION(NetMulticast, Reliable)
	virtual void BroadcastDisableCollision();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
//Damage Interface Override
	//virtual void Damage(FDamageData* DamageData) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

	//Montage 
	void AfterABPBindDynamic();
	
private:
	//Hit
	virtual void ApplyHitData();
	UFUNCTION(NetMulticast, Reliable)
		void BroadCastApplyHitData(FDamageData InDamageData);
	virtual void Die();
	virtual void RemoveCharacter();

	//Material 
	virtual  void CreateDynamicMaterial();
	virtual  void ChangeMeshColor(FLinearColor InColor);
	void ResetColor();
	

	//etc
	UFUNCTION()
	void OnStateTypeChangedHandler(EStateType PrevType, EStateType NewType);
	void RotateMeshToSlope(float InDeltaTime);

	//Montage
	UFUNCTION()
	void OnMontageFinalEnded();
	UFUNCTION()
	void OnMontageInterrupted();
	UFUNCTION()
	void OnPlayMontageNotifyBegin();
	UFUNCTION()
	void OnPlayMontageNotifyEnd();


	

protected: // 하위 클래스에서 설정하고 동적 로딩하기 위해 Protected 설정
	//Mesh
	FString SkeletalMeshPath;
	FString AnimInstancePath; 
	FString BBAssetPath;
	

	//DoAction
	UPROPERTY(EditAnywhere)
	TArray<FDoActionData> DoActionDatas;

	//Special Action
	UPROPERTY(EditAnywhere)
	TArray<FDoActionData> DoSpecialActionDatas;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arrow")
	class UArrowComponent* SlopeCheckArrow;
	// Box Collision Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* BoxCollision;

private:
	class UCGameInstance* GameInstance;
	class UNetDriver* NetDriver;


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
	float TraceOffset = 100.0f;
	int32 AttackIdx = 0;
	
	//Damage
	FDamageData DamageData;
	FHitData* HitData;
	FTimerHandle DieTimerHandle;
	int32 hitCnt = 0;
	
	//Material
	FLinearColor OriginalMeshColor = FLinearColor::White;
	FTimerHandle ResetColorTimerHandle;
public:
//ForceInline Getter & Settter
FORCEINLINE uint8 GetTeamID() { return TeamID; }

FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }

//FORCEINLINE class ACPatrolPath* GetPatrolPath() { return PatrolPath; }


	
};
