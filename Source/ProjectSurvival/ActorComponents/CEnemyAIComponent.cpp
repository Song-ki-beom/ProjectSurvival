

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


EAIStateType UCEnemyAIComponent::GetType()
{
	return (EAIStateType)Blackboard->GetValueAsEnum(AIStateTypeKey);
}

void UCEnemyAIComponent::ChangeType(EAIStateType InType)
{
	EAIStateType prevType = GetType();
	Blackboard->SetValueAsEnum(AIStateTypeKey, (uint8)InType);
		OnAIStateTypeChanged.Broadcast(prevType, InType);
}
/////////////////////////////////////////////////////////////////////
bool UCEnemyAIComponent::IsWaitMode()
{
	return GetType() == EAIStateType::Wait;
}

bool UCEnemyAIComponent::IsApproachMode()
{
	return GetType() == EAIStateType::Approach;
}

bool UCEnemyAIComponent::IsActionMode()
{
	return GetType() == EAIStateType::Action;
}

bool UCEnemyAIComponent::IsRoamMode()
{
	return GetType() == EAIStateType::Roam;
}

bool UCEnemyAIComponent::IsHittedMode()
{
	return GetType() == EAIStateType::Hitted;
}

bool UCEnemyAIComponent::IsAvoidMode()
{
	return GetType() == EAIStateType::Avoid;
}

bool UCEnemyAIComponent::IsDeadMode()
{
	return GetType() == EAIStateType::Dead;
}

void UCEnemyAIComponent::SetWaitMode()
{
	ChangeType(EAIStateType::Wait);
}

void UCEnemyAIComponent::SetApproachMode()
{
	ChangeType(EAIStateType::Approach);
}

void UCEnemyAIComponent::SetActionMode()
{
	ChangeType(EAIStateType::Action);
}

void UCEnemyAIComponent::SetRoamMode()
{
	ChangeType(EAIStateType::Roam);
}

void UCEnemyAIComponent::SetHittedMode()
{
	ChangeType(EAIStateType::Hitted);
}

void UCEnemyAIComponent::SetAvoidMode()
{
	ChangeType(EAIStateType::Avoid);
}

void UCEnemyAIComponent::SetDeadMode()
{
	ChangeType(EAIStateType::Dead);
}
