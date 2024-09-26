

#include "ActorComponents/CStatusComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"


UCStatusComponent::UCStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CurrentHealth = MaxHealth;
	
}


void UCStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);




}

void UCStatusComponent::ApplyDamage(float InAmount)
{
	float NewHealth = CurrentHealth -InAmount;
	NewHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	BroadcastUpdataHealth(NewHealth);
}

void UCStatusComponent::BroadcastUpdataHealth_Implementation(float NewHealth)
{
	CurrentHealth = NewHealth;
	OnHealthUpdated.Broadcast(CurrentHealth / MaxHealth);

}