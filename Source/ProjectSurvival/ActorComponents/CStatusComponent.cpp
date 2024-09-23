

#include "ActorComponents/CStatusComponent.h"
#include "GameFramework/Character.h"


UCStatusComponent::UCStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	Health = MaxHealth;
	
}


void UCStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);




}

void UCStatusComponent::ApplyDamage(float InAmount)
{
	Health -= InAmount;
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);

}

