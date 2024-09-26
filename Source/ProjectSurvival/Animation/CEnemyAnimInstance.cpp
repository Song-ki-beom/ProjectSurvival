// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/CEnemyAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ActorComponents/CStateComponent.h"
#include "Utility/CDebug.h"
#include "Net/UnrealNetwork.h"
#include "Character/CSurvivor.h"

void UCEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	if (OwnerCharacter == nullptr)
	{
		CDebug::Print(TEXT("No Character Found In ABP"));
		return;
	}
	bCastEnded = true;
	RootMotionMode = ERootMotionMode::IgnoreRootMotion;
	StateComponent = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));
	StateComponent->OnStateTypeChanged.AddDynamic(this, &UCEnemyAnimInstance::OnStateTypeChangedHandler);
}

void UCEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!bCastEnded)
		return;

	if (IsValid(OwnerCharacter))
	{
		Speed = UKismetMathLibrary::FInterpTo(Speed, OwnerCharacter->GetVelocity().Size2D(), DeltaSeconds, 25);
		/*FRotator characterRotation = OwnerCharacter->GetActorRotation();
		FVector worldVelocity = OwnerCharacter->GetMovementComponent()->Velocity;
		FVector localVelocity = characterRotation.UnrotateVector(worldVelocity);
		Direction = CalculateDirection(localVelocity, FRotator(0, 0, 0));*/

		FRotator CharacterRotator = OwnerCharacter->GetVelocity().ToOrientationRotator();
		FRotator Controlrotator = OwnerCharacter->GetControlRotation();
		FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotator, Controlrotator); //두 Rotator 간의 각도 차이 계산 및 -180 ~180 도 사이로 정규화
		PrevRotation = UKismetMathLibrary::RInterpTo(PrevRotation, delta, DeltaSeconds, 30.0f); //20의 보간 속도로 delta 방향으로 direction 설정
		Direction = PrevRotation.Yaw; //최종 방향 설정 


	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerCharacter is not valid - UCEnemyAnimInstance"));
	}
}

void UCEnemyAnimInstance::OnStateTypeChangedHandler(EStateType prevType, EStateType NewType)
{
	StateType = NewType;
}


//추후 진행할 리플리케이트를 위해 
//void UCEnemyAnimInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//	DOREPLIFETIME(UCSurvivorAnimInstance, WeaponType);
//
//
//}

