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
	UFUNCTION()
		void ChangeTarget(class AActor* InTarget);
	UFUNCTION()
		void RunAI();
	UFUNCTION()
		void StopAI();
protected:  
	virtual void BeginPlay() override;
protected:
	void  OnPossess(APawn* InPawn)  override; 
	void  OnUnPossess()   override;
private:
	UPROPERTY(VisibleAnywhere)
		class UAIPerceptionComponent* Perception;
private:  
	UFUNCTION()
		void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	UFUNCTION()
		void EnableAggroCoolDown();
	
private: 
	class  ACEnemy* Enemy;
	class  UAISenseConfig_Sight* Sight;
	class  UCEnemyAIComponent* AIComponent;

	bool bIsAggroCoolDown = true;
	float CooldownTime =  5.0f;
	FTimerHandle AggroTimerHandle;
	TArray<AActor*> CandidateActors;
	AActor* TargetActor;
};
