#include "ActorComponents/CMovingComponent.h"
#include  "GameFramework/CharacterMovementComponent.h"
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

	OwnerCharacter = Cast<ACharacter>(this->GetOwner());
}


// Called every frame
void UCMovingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	float CurrentSpeed = OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed;
	if (CurrentSpeed  != TargetSpeed)
	{
		float NewSpeed = FMath::FInterpTo(CurrentSpeed, TargetSpeed, DeltaTime, SpeedInterpRate);
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
	}

}

void UCMovingComponent::OnMoveForward(float InAxisValue)
{
	if (!bCanMove) return;
	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();

	OwnerCharacter->AddMovementInput(direction, InAxisValue);
}

void UCMovingComponent::OnMoveRight(float InAxisValue)
{
	if (!bCanMove) return;
	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();

	OwnerCharacter->AddMovementInput(direction, InAxisValue);
}

void UCMovingComponent::OnHorizontalLook(float InAxisValue)
{
	OwnerCharacter->AddControllerYawInput(InAxisValue * 0.75f);
}

void UCMovingComponent::OnVerticalLook(float InAxisValue)
{
	OwnerCharacter->AddControllerPitchInput(InAxisValue * 0.75f);
}

void UCMovingComponent::EnableControlRotation()
{
	OwnerCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = true;
	//OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false; //캐릭터가 이동 방향에 따라 자동으로 회전하지 않음 
}

void UCMovingComponent::DisableControlRotation()
{
	// 캐릭터가 컨트롤러의 회전을 따르지 않도록 설정
	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true; //캐릭터가 이동 방향에 따라 자동으로 회전하지 않음 
}

void UCMovingComponent::SetSpeed(ESpeedType InType)
{
	switch (InType)
	{
	case ESpeedType::Walk:
		TargetSpeed = GetWalkSpeed();
		break;
	case ESpeedType::Run:
		TargetSpeed = GetRunSpeed();
		break;
	case ESpeedType::Sprint:
		TargetSpeed = GetSprintSpeed();
		break;
	}

	

}

void UCMovingComponent::OnWalk()
{
	SetSpeed(ESpeedType::Walk);
}

void UCMovingComponent::OnRun()
{
	SetSpeed(ESpeedType::Run);
}

void UCMovingComponent::OnSprint()
{
	SetSpeed(ESpeedType::Sprint);
}