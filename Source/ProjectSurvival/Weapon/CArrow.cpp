// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/CArrow.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/CapsuleComponent.h"

ACArrow::ACArrow()
{
	bReplicates = true;
	SetReplicates(true);
	PrimaryActorTick.bCanEverTick = true;
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	Projectile->ProjectileGravityScale = 0.0f;              
	Capsule->BodyInstance.bNotifyRigidBodyCollision = true;   
	Capsule->SetCollisionProfileName("BlockAll");
}
	

void ACArrow::BeginPlay()
{
	Super::BeginPlay();

	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Capsule->OnComponentHit.AddDynamic(this, &ACArrow::OnComponentHit);
	Projectile->Deactivate();
}

void ACArrow::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (OnEndPlay.IsBound())
		OnEndPlay.Broadcast(this);
}

void ACArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotateArrow();

}

void ACArrow::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	SetLifeSpan(LifeSpanAfterCollision);

	for (AActor* actor : Ignores)
		if (actor == OtherActor) return;

	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ACharacter* character = Cast<ACharacter>(OtherActor);
	if(character == nullptr) return;

	if (OnHit.IsBound())
		OnHit.Broadcast(this, character);

	Destroy();
}

void ACArrow::Shoot(const FVector& InFoward)
{
	if(GetAttachParentActor() )//GetAttachParentActor() && !Arrow->GetAttachParentActor()->IsPendingKill()
		DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	Projectile->Velocity = InFoward * Projectile->InitialSpeed;
	//RotateArrow();
	Projectile->Activate();
	Projectile->bRotationFollowsVelocity = true;
	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

}


//-----------------------------------
void ACArrow::RequestShoot_Implementation(const FVector& InFoward)
{
	BroadcastShoot(InFoward);
}


void ACArrow::BroadcastShoot_Implementation(const FVector& InFoward)
{
	DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	Projectile->Velocity = InFoward * Projectile->InitialSpeed;
	//RotateArrow();
	Projectile->Activate();
	Projectile->bRotationFollowsVelocity = true;
	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACArrow::RotateArrow()
{
	//FRotator ProjectileRotator = Projectile->Velocity.Rotation();
	//FRotator newRotator = FRotator(ProjectileRotator.Pitch, GetActorRotation().Yaw, GetActorRotation().Roll);
	//SetActorRotation(newRotator);
}

