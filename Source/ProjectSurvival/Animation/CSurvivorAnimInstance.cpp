#include "CSurvivorAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Utility/CDebug.h"
#include "Character/CSurvivor.h"

void UCSurvivorAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	if (OwnerCharacter == nullptr) 
	{
		CDebug::Print(TEXT("No Character Found In ABP"));
		return;
	}
	bCastEnded = true;

	Weapon = Cast<UCWeaponComponent>(OwnerCharacter->GetComponentByClass(UCWeaponComponent::StaticClass()));
	if (!!Weapon)
		Weapon->OnWeaponTypeChanged.AddDynamic(this, &UCSurvivorAnimInstance::OnWeaponTypeChanged);
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

void UCSurvivorAnimInstance::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	WeaponType = InNewType;
}

