

#include "ActorComponents/CEnemyAIComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UCEnemyAIComponent::UCEnemyAIComponent()
{

}

void UCEnemyAIComponent::BeginPlay()
{
	Super::BeginPlay();
	

}




ACharacter* UCEnemyAIComponent::GetTarget()
{
	return Cast<ACharacter>(Blackboard->GetValueAsObject(TargetKey));
}

FVector UCEnemyAIComponent::GetRoamingLocation()
{
	return Blackboard->GetValueAsVector(RoamLocationKey);
}

void UCEnemyAIComponent::SetRoamingLocation(const FVector& InLocation)
{
	Blackboard->SetValueAsVector(RoamLocationKey, InLocation);
}


EAIStateType UCEnemyAIComponent::GetAIStateType()
{
	if(Blackboard)
		return (EAIStateType)Blackboard->GetValueAsEnum(AIStateTypeKey);

	return EAIStateType::Max;

}

EAIReputationType UCEnemyAIComponent::GetAIReputationType()
{
	if(Blackboard)
		return(EAIReputationType)Blackboard->GetValueAsEnum(AIReputationTypeKey);
	
	return EAIReputationType::Max;

}

void UCEnemyAIComponent::ChangeAIStateType(EAIStateType InType)
{
	EAIStateType prevType = GetAIStateType();
	Blackboard->SetValueAsEnum(AIStateTypeKey, (uint8)InType);
	if(OnAIStateTypeChanged.IsBound())
		OnAIStateTypeChanged.Broadcast(prevType, InType);
}
void UCEnemyAIComponent::ChangeAIReputationType(EAIReputationType InType)
{
	EAIReputationType prevType = GetAIReputationType();
	Blackboard->SetValueAsEnum(AIReputationTypeKey, (uint8)InType);
	if(OnAIReputationTypeChanged.IsBound())
		OnAIReputationTypeChanged.Broadcast(prevType, InType);
}


bool UCEnemyAIComponent::IsWaitMode()
{
	return GetAIStateType() == EAIStateType::Wait;
}

bool UCEnemyAIComponent::IsApproachMode()
{
	return GetAIStateType() == EAIStateType::Approach;
}

bool UCEnemyAIComponent::IsActionMode()
{
	return GetAIStateType() == EAIStateType::Action;
}

bool UCEnemyAIComponent::IsRoamMode()
{
	return GetAIStateType() == EAIStateType::Roam;
}

bool UCEnemyAIComponent::IsHittedMode()
{
	return GetAIStateType() == EAIStateType::Hitted;
}

bool UCEnemyAIComponent::IsAvoidMode()
{
	return GetAIStateType() == EAIStateType::Avoid;
}

bool UCEnemyAIComponent::IsDeadMode()
{
	return GetAIStateType() == EAIStateType::Dead;
}

bool UCEnemyAIComponent::IsExhaustMode()
{
	return GetAIStateType() == EAIStateType::Exhaust;
}

bool UCEnemyAIComponent::IsHostileMode()
{
	return GetAIReputationType() == EAIReputationType::Hostile;
}

bool UCEnemyAIComponent::IsNeutralMode()
{
	return GetAIReputationType() == EAIReputationType::Neutral;
}

bool UCEnemyAIComponent::IsFriendlyMode()
{
	return GetAIReputationType() == EAIReputationType::Friendly;
}

void UCEnemyAIComponent::SetWaitMode()
{
	ChangeAIStateType(EAIStateType::Wait);
}

void UCEnemyAIComponent::SetApproachMode()
{
	ChangeAIStateType(EAIStateType::Approach);
}

void UCEnemyAIComponent::SetActionMode()
{
	ChangeAIStateType(EAIStateType::Action);
}

void UCEnemyAIComponent::SetRoamMode()
{
	ChangeAIStateType(EAIStateType::Roam);
}

void UCEnemyAIComponent::SetHittedMode()
{
	ChangeAIStateType(EAIStateType::Hitted);
}

void UCEnemyAIComponent::SetAvoidMode()
{
	ChangeAIStateType(EAIStateType::Avoid);
}

void UCEnemyAIComponent::SetDeadMode()
{
	ChangeAIStateType(EAIStateType::Dead);
}

void UCEnemyAIComponent::SetExhaustMode()
{
	ChangeAIStateType(EAIStateType::Exhaust);
}

void UCEnemyAIComponent::SetHostileMode()
{
	ChangeAIReputationType(EAIReputationType::Hostile);
}

void UCEnemyAIComponent::SetNeutralMode()
{
	ChangeAIReputationType(EAIReputationType::Neutral);
}

void UCEnemyAIComponent::SetFriendlyMode()
{
	ChangeAIReputationType(EAIReputationType::Friendly);
}
