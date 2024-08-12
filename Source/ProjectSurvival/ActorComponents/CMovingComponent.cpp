#include "ActorComponents/CMovingComponent.h"
#include "Character/CSurvivor.h"

// Sets default values for this component's properties
UCMovingComponent::UCMovingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCMovingComponent::BeginPlay()
{
	Super::BeginPlay();

	CSurvivor = Cast<ACSurvivor>(this->GetOwner());
}


// Called every frame
void UCMovingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCMovingComponent::OnMoveForward(float InAxisValue)
{
	FRotator rotator = FRotator(0, CSurvivor->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();

	CSurvivor->AddMovementInput(direction, InAxisValue);
}

void UCMovingComponent::OnMoveRight(float InAxisValue)
{
	FRotator rotator = FRotator(0, CSurvivor->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();

	CSurvivor->AddMovementInput(direction, InAxisValue);
}

void UCMovingComponent::OnHorizontalLook(float InAxisValue)
{
	CSurvivor->AddControllerYawInput(InAxisValue * 0.75f);
}

void UCMovingComponent::OnVerticalLook(float InAxisValue)
{
	CSurvivor->AddControllerPitchInput(InAxisValue * 0.75f);
}