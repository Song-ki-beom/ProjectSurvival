#include "CSurvivorAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/CSurvivor.h"

void UCSurvivorAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	bCastEnded = true;
}

void UCSurvivorAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!bCastEnded)
		return;

	if (IsValid(OwnerCharacter))
	{
		Speed = UKismetMathLibrary::FInterpTo(Speed, OwnerCharacter->GetVelocity().Size2D(), DeltaSeconds, 25);

		FRotator characterRotation = OwnerCharacter->GetActorRotation();
		FVector worldVelocity = OwnerCharacter->GetMovementComponent()->Velocity;
		FVector localVelocity = characterRotation.UnrotateVector(worldVelocity);
		Direction = CalculateDirection(localVelocity, FRotator(0, 0, 0));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerCharacter is not valid - UCSurvivorAnimInstance"));
	}
}

