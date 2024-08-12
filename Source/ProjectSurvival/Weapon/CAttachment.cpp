// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/CAttachment.h"
#include "GameFramework/Character.h"
#include "Components/SceneComponent.h"
#include "Components/ShapeComponent.h"    




ACAttachment::ACAttachment()
{
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh2"));
	Mesh->SetupAttachment(Root);

}


void ACAttachment::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	TArray<USceneComponent*> children;
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Root->GetChildrenComponents(true, children);

	AttachTo("socket_backpack");
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	Super::BeginPlay();
}


void ACAttachment::OnBeginEquip_Implementation()
{
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
}

void ACAttachment::OnUnEquip_Implementation()
{
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
}

void ACAttachment::AttachTo(FName InSocketName)
{
	if(!OwnerCharacter) return;
	AttachToComponent(OwnerCharacter->GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::KeepRelative, true),
		InSocketName);
}




