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

	UFUNCTION()
		void ChangeTarget(class AActor* InTarget);
	UFUNCTION()
		void ChangeFriendlyTarget(AActor* InTarget);

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
		void EnableAggroCoolDown();
	UFUNCTION()
		void CustomTick();

protected:
	UPROPERTY()
		FGenericTeamId TeamId; // 팀 ID 저장 변수

private:
	UPROPERTY(VisibleAnywhere)
		class UAIPerceptionComponent* Perception;
 
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
};
