// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/CAttachment_Bow.h"
#include "Animation/CBowAnimInstance.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "ActorComponents/CStateComponent.h"
#include "Components/BoxComponent.h"


ACAttachment_Bow::ACAttachment_Bow()
{
	this->PrimaryActorTick.bCanEverTick = true;

	Mesh->SetVisibility(false);
	HitBox->SetVisibility(false);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	SkeletalMesh->SetupAttachment(Root);

	//PoseableMesh = CreateDefaultSubobject<UPoseableMeshComponent>("PoseableMesh");
	//PoseableMesh->SetupAttachment(Root);


	USkeletalMesh* mesh;
	ConstructorHelpers::FObjectFinder<USkeletalMesh> asset(TEXT("SkeletalMesh'/Game/PirateIsland/Include/Animation/AnimationSequence/Weapon/SK_Bow.SK_Bow'"));
	mesh = asset.Object;
	SkeletalMesh->SetSkeletalMesh(mesh);
	SkeletalMesh->SetCollisionProfileName("NoCollision"); 

}



void ACAttachment_Bow::BeginPlay()
{
	Super::BeginPlay();

	//AttachTo("Holster_Bow");

	//SkeletalMesh->SetVisibility(false);
	//PoseableMesh->SetSkeletalMesh(SkeletalMesh->SkeletalMesh);
	//PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);
	//SkeletalMesh->SetVisibility(false);

}

void ACAttachment_Bow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACAttachment_Bow::OnBeginEquip_Implementation()
{
	Super::OnBeginEquip_Implementation();
	//AttachTo("Hand_Bow_Left");

	APlayerController* contoller = OwnerCharacter->GetController<APlayerController>();
	if(contoller == nullptr) return;

	OriginViewPitchRange.X = contoller->PlayerCameraManager->ViewPitchMin;
	OriginViewPitchRange.Y = contoller->PlayerCameraManager->ViewPitchMax;

	contoller->PlayerCameraManager->ViewPitchMin = ViewPitchRange.X;
	contoller->PlayerCameraManager->ViewPitchMax = ViewPitchRange.Y;

}

void ACAttachment_Bow::OnUnEquip_Implementation()
{
	Super::OnUnEquip_Implementation();
	//AttachTo("Holster_Bow");


	APlayerController* contoller = OwnerCharacter->GetController<APlayerController>();
	if (contoller == nullptr) return;


	contoller->PlayerCameraManager->ViewPitchMin = OriginViewPitchRange.X;
	contoller->PlayerCameraManager->ViewPitchMax = OriginViewPitchRange.Y;

}

float* ACAttachment_Bow::GetBend()
{
	
	return Cast<UCBowAnimInstance>(SkeletalMesh->GetAnimInstance())->GetBend();
}
