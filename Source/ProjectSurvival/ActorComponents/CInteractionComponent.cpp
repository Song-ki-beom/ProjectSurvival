

#include "ActorComponents/CInteractionComponent.h"
#include "GameFramework/Character.h"

UCInteractionComponent::UCInteractionComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UCInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());

}


// Called every frame
void UCInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCInteractionComponent::PerformInteractionCheck()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	FVector TraceStart{ OwnerCharacter->GetPawnViewLocation()}; //트레이스 벡터 초기화 
	


}

void UCInteractionComponent::FoundInteractable(AActor* NewInteractable)
{

}

void UCInteractionComponent::NoInteractableFound()
{

}

void UCInteractionComponent::EndInteract()
{

}

void UCInteractionComponent::Interact()
{

}




