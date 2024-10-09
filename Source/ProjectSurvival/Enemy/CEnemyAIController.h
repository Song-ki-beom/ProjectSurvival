// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API ACEnemyAIController : public AAIController
{
	GENERATED_BODY()
public:
	ACEnemyAIController();
	// 팀 간의 관계를 정의하는 함수 (IGenericTeamAgentInterface에서 상속)
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamId) override;
	//virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	/*UFUNCTION()
		void RetryMoveToLocation();*/

	UFUNCTION()
	void UpdatePerception();
	UFUNCTION()
	void TameEnemyToPlayer(class APlayerController* PlayerController);
	UFUNCTION()
	void ForceUpdatePerception();
	UFUNCTION()
	void EnableAggroCoolDown();

	UFUNCTION()
		void ChangeTargetLocation(FVector InTargetLocation);
	UFUNCTION()
		FVector GetCurrentTargetLocation();
		
	UFUNCTION()
		void ChangeTarget(class AActor* InTarget);
	UFUNCTION()
		class AActor* GetTarget();
	UFUNCTION()
		void ChangeFriendlyTarget(AActor* InTarget);
	UFUNCTION()
		class AActor* GetFriendlyTarget();

	UFUNCTION()
		void RunAI();
	UFUNCTION()
		void StopAI();
	

protected:  
	virtual void BeginPlay() override;
	void  OnPossess(APawn* InPawn)  override; 
	void  OnUnPossess()   override;

private:
	UFUNCTION()
		void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	UFUNCTION()
		void CustomTick();

protected:
	UPROPERTY()
		FGenericTeamId TeamId; // 팀 ID 저장 변수

private:
	UPROPERTY(VisibleAnywhere)
		class UAIPerceptionComponent* Perception;
	UPROPERTY()
		class UBehaviorTreeComponent* BehaviorTreeComp;
	
	bool bIsBehaviorTreeInitialized;
 
	class  ACEnemy* Enemy;
	class  UAISenseConfig_Sight* Sight;
	class  UCEnemyAIComponent* AIComponent;
	FTimerHandle TickTimerHandle;

	bool bIsAggroCoolDown = true;
	float CooldownTime =  3.0f;
	FTimerHandle AggroTimerHandle;
	TArray<AActor*> CandidateActors;
	AActor* TargetActor;
	AActor* FriendlyTargetActor;
	FVector TargetLocation;

	
};
