#include "Weapon/CAttachment.h"
#include "GameFramework/Character.h"
#include "Components/SceneComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/BoxComponent.h"
#include "Utility/CDebug.h"

ACAttachment::ACAttachment()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh2"));
	Mesh->SetupAttachment(Root);
	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
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

	if (Mesh->DoesSocketExist(FName("HitSocket")))
	{
		HitBox->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("HitSocket"));
	}
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