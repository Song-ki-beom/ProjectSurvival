// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CEnemyAIComponent.generated.h"


UENUM(BlueprintType)
enum class EAIStateType : uint8
{
	Wait, Approach, Action, Roam, Hitted, Avoid, Dead, Exhaust,Following,Max
};

UENUM(BlueprintType)
enum class EAIReputationType : uint8
{
	Hostile, Neutral, Friendly,Max
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAIStateTypeChanged, EAIStateType, InPrevType, EAIStateType, InNewType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAIReputationTypeChanged, EAIReputationType, InPrevType, EAIReputationType, InNewType);


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
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
		FName AIReputationTypeKey = "AIReputation";
	UPROPERTY(EditAnywhere, Category = "Key")
		FName TargetKey = "Target";
	UPROPERTY(EditAnywhere, Category = "Key")
		FName FriendlyTargetKey = "FriendlyTarget";
	UPROPERTY(EditAnywhere, Category = "Key")
		FName RoamLocationKey = "Roam_Location";

public:
	class  ACharacter* GetTarget();
	class ACharacter* GetFriendlyTarget();
	FVector GetRoamingLocation();
	void    SetRoamingLocation(const FVector& InLocation);
	void    ChangeAIStateType(EAIStateType InType);
	void    ChangeAIReputationType(EAIReputationType InType);

private:
	EAIStateType  GetAIStateType();
	EAIReputationType GetAIReputationType();

public:

	//AIState
	bool IsWaitMode();
	bool IsApproachMode();
	bool IsActionMode();
	bool IsRoamMode();
	bool IsHittedMode();
	bool IsAvoidMode();
	bool IsDeadMode();
	bool IsExhaustMode();
	bool IsFollowingMode();

	//AIReputation
	bool IsHostileMode();
	bool IsNeutralMode();
	bool IsFriendlyMode();

public:
	//AIState
	void SetWaitMode();
	void SetApproachMode();
	void SetActionMode();
	void SetRoamMode();
	void SetHittedMode();
	void SetAvoidMode();
	void SetDeadMode();
	void SetExhaustMode();
	void SetFollowingMode();

	//AIReputation
	void SetHostileMode();
	void SetNeutralMode();
	void SetFriendlyMode();

public: 
	FAIStateTypeChanged OnAIStateTypeChanged;
	FAIReputationTypeChanged OnAIReputationTypeChanged;
private:
	class UBlackboardComponent* Blackboard; 

public:
	FORCEINLINE void SetBlackboard(class UBlackboardComponent* InBlackboard)
	{
		Blackboard = InBlackboard;
	}
	FORCEINLINE UBlackboardComponent* GetBlackboard()
	{
		return Blackboard;
	}

};


		

