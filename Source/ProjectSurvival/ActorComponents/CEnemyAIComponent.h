// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CEnemyAIComponent.generated.h"


UENUM(BlueprintType)
enum class EAIStateType : uint8
{
	Wait, Approach, Action, Roam, Hitted, Avoid, Dead, Max
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAIStateTypeChanged, EAIStateType, InPrevType, EAIStateType, InNewType);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSURVIVAL_API UCEnemyAIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCEnemyAIComponent();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Key")
		FName AIStateTypeKey = "AIState";
	UPROPERTY(EditAnywhere, Category = "Key")
		FName TargetKey = "Target";
	UPROPERTY(EditAnywhere, Category = "Key")
		FName RoamLocationKey = "Roam_Location";

public:
	class  ACharacter* GetTarget();
	FVector GetRoamingLocation();
	void    SetRoamingLocation(const FVector& InLocation);
	void    ChangeType(EAIStateType InType);
private:
	EAIStateType  GetType();
	

public:
	bool IsWaitMode();
	bool IsApproachMode();
	bool IsActionMode();
	bool IsRoamMode();
	bool IsHittedMode();
	bool IsAvoidMode();
	bool IsDeadMode();

public:
	void SetWaitMode();
	void SetApproachMode();
	void SetActionMode();
	void SetRoamMode();
	void SetHittedMode();
	void SetAvoidMode();
	void SetDeadMode();



public: 
	FAIStateTypeChanged OnAIStateTypeChanged;

private:
	class UBlackboardComponent* Blackboard; 

public:
	FORCEINLINE void SetBlackboard(class UBlackboardComponent* InBlackboard)
	{
		Blackboard = InBlackboard;
	}


};


		

