// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/CArrow.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"

ACArrow::ACArrow()
{
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
}

void ACArrow::Shoot(const FVector& InFoward)
{
	Projectile->Velocity = InFoward * Projectile->InitialSpeed;
	Projectile->Activate();
	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}


