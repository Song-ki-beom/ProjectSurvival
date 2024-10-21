#include "CSurvivorAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Utility/CDebug.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/CSubAction.h"
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
	//StateComponent = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));
	//StateComponent->OnStateTypeChanged.AddDynamic(this, &UCEnemyAnimInstance::OnStateTypeChangedHandler);

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
		Rotation = characterRotation;
		FVector worldVelocity = OwnerCharacter->GetMovementComponent()->Velocity;
		FVector localVelocity = characterRotation.UnrotateVector(worldVelocity);
		Direction = CalculateDirection(localVelocity, FRotator(0, 0, 0));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerCharacter is not valid - UCSurvivorAnimInstance"));
	}

	float NewPitch = UKismetMathLibrary::FInterpTo(Pitch, OwnerCharacter->GetBaseAimRotation().Pitch, DeltaSeconds, 25);
	if (!OwnerCharacter->IsLocallyControlled())
	{
		const FVector AimDirWS = OwnerCharacter->GetBaseAimRotation().Vector(); //월드 공간의 rotation
		const FVector AimDirLS = OwnerCharacter->ActorToWorld().InverseTransformVectorNoScale(AimDirWS);//월드 공간->로컬 공간으로 변환..월드 좌표에서의 회전값과 로컬 좌표에서의 회전값이 다르게 해석되었기 때문임
		const FRotator AimRotLS = AimDirLS.Rotation();
		NewPitch = AimRotLS.Pitch;
	}
	Pitch = NewPitch;


	/*if (OwnerCharacter->HasAuthority())
	{
		
		CDebug::Print(TEXT("Client Pitch: "), Pitch);
	}
	else
	{
		RequestSetPitch(NewPitch);
		CDebug::Print(TEXT("Server Pitch: "), Pitch);
	}*/

	bFalling = OwnerCharacter->GetCharacterMovement()->IsFalling();

	if(Weapon ==nullptr) return;
		bBow_Aiming = false;
		if (!!Weapon->GetSubAction())
		{
			if (WeaponType != EWeaponType::Bow) bBow_Aiming = true;
			if (Weapon->GetSubAction()->GetInAction()) bBow_Aiming = true;

		}
	
	


}

void UCSurvivorAnimInstance::SetPitch(float InNewPitch)
{
	/*if (!OwnerCharacter->IsLocallyControlled())
	{
		NewPitch = OwnerCharacter->RemoteViewPitch * 360.f / 255.f;
	}
	Pitch = InNewPitch;*/
}

void UCSurvivorAnimInstance::RequestSetPitch_Implementation(float InNewPitch)
{
	Pitch = InNewPitch;
}




void UCSurvivorAnimInstance::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	WeaponType = InNewType;
}

void UCSurvivorAnimInstance::OnRep_WeaponTypeChanged()
{
	//CDebug::Print(TEXT("On AnimInstance WeaponType Changed"));

}

void UCSurvivorAnimInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCSurvivorAnimInstance, WeaponType);
	DOREPLIFETIME(UCSurvivorAnimInstance, bBow_Aiming);
}

