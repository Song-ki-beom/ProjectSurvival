// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/CSubAction_Bow.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "ActorComponents/CAmmoComponent.h"
#include "ActorComponents/CMontageComponent.h"
#include "Animation/CBowAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "ActorComponents/CStateComponent.h"
#include "Utility/CDebug.h"
#include "Weapon/CDoAction.h"
#include "Weapon/CDoAction_Bow.h"
#include "Weapon/CAttachment_Bow.h"
#include "Widget/CMainHUD.h"
#include "CGameInstance.h"
#include "Widget/Map/CWorldMap.h"
#include "Character/CSurvivorController.h"

UCSubAction_Bow::UCSubAction_Bow()
{

}
void UCSubAction_Bow::BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCEquipment* InEquipment, class UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InEquipment, InDoAction);

	AmmoComponent = Cast<UCAmmoComponent>(InOwner->GetComponentByClass(UCAmmoComponent::StaticClass()));
	SpringArm = Cast<USpringArmComponent>(InOwner->GetComponentByClass(USpringArmComponent::StaticClass()));
	Camera = Cast<UCameraComponent>(InOwner->GetComponentByClass(UCameraComponent::StaticClass()));		
	MontageComponent = Cast<UCMontageComponent>(InOwner->GetComponentByClass(UCMontageComponent::StaticClass()));
	if (MontageComponent)
	{
		MontageComponent->OnMontageFinalEnded.AddDynamic(this, &UCSubAction_Bow::OnMontageEnded);
	}

	OwnerCharacter = InOwner;

	FOnTimelineVector timeLine;
	timeLine.BindUFunction(this, "OnAiming");
	Timeline.AddInterpVector(Curve, timeLine);
	Timeline.SetPlayRate(AimingSpeed);
	
	ACAttachment_Bow* bow = Cast<ACAttachment_Bow>(InAttachment);
	AnimInstance_Bow = Cast<UCBowAnimInstance>(bow->SkeletalMesh->GetAnimInstance());
	
	//ABP Bow 의 Bend 값 
	if (bow)
		Bend = bow->GetBend();

}
void  UCSubAction_Bow::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);
	Timeline.TickTimeline(InDeltaTime);
}




void UCSubAction_Bow::OnMontageEnded()
{
	//Released();
}

void UCSubAction_Bow::Pressed()
{
	if(State->IsSubActionMode()) return;
	if(SpringArm==nullptr) return;
	if(Camera==nullptr) return;
	
	Super::Pressed();
	
	UCGameInstance* gameInstance = Cast<UCGameInstance>(OwnerCharacter->GetWorld()->GetGameInstance());
	if (gameInstance)
	{
		ACMainHUD* mainHUD = Cast<ACMainHUD>(gameInstance->WorldMap->GetPersonalSurvivorController()->GetHUD());
		if (mainHUD)
			mainHUD->ShowCrossHair();
	}

	State->OnSubActionMode();
	{
		//OriginData.TargetArmLength = SpringArm->TargetArmLength;
		OriginData.SocketOffset = SpringArm->SocketOffset;
		OriginData.bEnableCameraLag = SpringArm->bEnableCameraLag;
		OriginData.CameraLocation = Camera->GetRelativeLocation();
	}
	{
		//SpringArm->TargetArmLength = AimData.TargetArmLength;
		SpringArm->SocketOffset = AimData.SocketOffset;
		SpringArm->bEnableCameraLag = AimData.bEnableCameraLag;
		Camera->SetRelativeLocation(AimData.CameraLocation);
	}
	//AnimInstance_Bow->SetAiming(true);


	Timeline.PlayFromStart();
	
	if (AmmoComponent)
	{
		AmmoComponent->ShowAttachedArrow();
	}
	
}

void UCSubAction_Bow::Released()
{
	if(!State->IsSubActionMode()) return;
	if(SpringArm == nullptr) return;
	if(Camera == nullptr) return;

	Super::Released();

	UCGameInstance* gameInstance = Cast<UCGameInstance>(OwnerCharacter->GetWorld()->GetGameInstance());
	if (gameInstance)
	{
		ACMainHUD* mainHUD = Cast<ACMainHUD>(gameInstance->WorldMap->GetPersonalSurvivorController()->GetHUD());
		if (mainHUD)
			mainHUD->HideCrossHair();
	}

	State->OffSubActionMode();

	{
		//SpringArm->TargetArmLength = OriginData.TargetArmLength;
		SpringArm->SocketOffset = OriginData.SocketOffset;
		SpringArm->bEnableCameraLag = OriginData.bEnableCameraLag;
		Camera->SetRelativeLocation(OriginData.CameraLocation);
	}
	//Camera->SetFieldOfView(90.0f);
	//AnimInstance_Bow->SetAiming(false);

	Timeline.ReverseFromEnd();

	if (AmmoComponent)
	{
		AmmoComponent->HideAttachedArrow();
	}
}

void UCSubAction_Bow::OnAiming(FVector Output)
{
	Camera->FieldOfView = Output.X;
	//CDebug::Print("Camera FieldOfView",Camera->FieldOfView);
	if (!!Bend)
		*Bend = Output.Y;
}
