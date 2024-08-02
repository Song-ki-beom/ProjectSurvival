﻿#include "CSurvivor.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputCoreTypes.h"
#include "DrawDebugHelpers.h"

ACSurvivor::ACSurvivor()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = this->CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera = this->CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	// Skeletal Mesh
	USkeletalMesh* skeletalMesh = nullptr;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMeshFinder(TEXT("SkeletalMesh'/Game/PirateIsland/Include/Skeletal/Character/Survivor/SK_Survivor.SK_Survivor'"));
	if (skeletalMeshFinder.Succeeded())
	{
		skeletalMesh = skeletalMeshFinder.Object;
		GetMesh()->SetSkeletalMesh(skeletalMesh);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("skeletalMeshFinder Failed - ACSurvivor"));
	}

	// AnimInstance
	//static ConstructorHelpers::FClassFinder<UAnimInstance> animInstanceFinder(TEXT("AnimBlueprint'/Game/PirateIsland/Include/Animation/AnimationBlueprint/ABP_CSurvivor.ABP_CSurvivor_C'"));
	//if (animInstanceFinder.Succeeded())
	//{
	//	GetMesh()->SetAnimClass(animInstanceFinder.Class);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("animInstanceFinder Failed - ACSurvivor"));
	//}

	bUseControllerRotationYaw = true;
	// true일 경우 컨트롤러의 회전 방향으로 캐릭터가 회전한다.
	// false일 경우 캐릭터의 이동 방향으로 캐릭터가 회전한다.
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = 450;

	SpringArm->SetRelativeLocation(FVector(0, 0, 100));
	SpringArm->TargetArmLength = 400;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;

	
}

void ACSurvivor::BeginPlay()
{
	Super::BeginPlay();
	//SpringArm->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}



void ACSurvivor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACSurvivor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACSurvivor::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACSurvivor::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACSurvivor::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACSurvivor::OnVerticalLook);
	//PlayerInputComponent->BindAction("Slash", this, &ACSurvivor::OnVerticalLook);

}

void ACSurvivor::OnMoveForward(float InAxisValue)
{
	FRotator rotator = FRotator(0, this->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();

	this->AddMovementInput(direction, InAxisValue);
}

void ACSurvivor::OnMoveRight(float InAxisValue)
{
	FRotator rotator = FRotator(0, this->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();

	this->AddMovementInput(direction, InAxisValue);
}

void ACSurvivor::OnHorizontalLook(float InAxisValue)
{
	this->AddControllerYawInput(InAxisValue * 0.75f);
}

void ACSurvivor::OnVerticalLook(float InAxisValue)
{
	this->AddControllerPitchInput(InAxisValue * 0.75f);
}

void ACSurvivor::SlashTree()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Character Slashing !"));
	
	FVector StartLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector EndLocation = StartLocation + (ForwardVector * TraceDistance);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()-> LineTraceSingleByChannel (
		HitResult,
		StartLocation,
		EndLocation,
		ECC_Visibility,
		CollisionParams
			);

	if (bHit)
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		//if (HitComponent);
	}
	

}


