// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/CArrow.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
#include "Utility/CDebug.h"
#include "Components/SceneComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/CapsuleComponent.h"

ACArrow::ACArrow()
{
	bReplicates = true;
	SetReplicates(true);
	PrimaryActorTick.bCanEverTick = true;
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	RootComponent = Capsule;
	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	//StaticMesh'/Game/PirateIsland/Include/Animation/AnimationSequence/Weapon/SM_Arrow.SM_Arrow'
	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));
	ArrowMesh->SetupAttachment(Capsule);
	ArrowMesh->SetCollisionProfileName("NoCollision");
	Projectile->InitialSpeed = 500000.0f;
	Projectile->MaxSpeed = 500000.0f;
	Projectile->ProjectileGravityScale = 0.0f;
	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	ParticleSystemComponent->AttachToComponent(ArrowMesh, FAttachmentTransformRules::KeepRelativeTransform);
	ParticleSystemComponent->bAutoActivate = false;  

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshAsset(TEXT("StaticMesh'/Game/PirateIsland/Include/Animation/AnimationSequence/Weapon/SM_Arrow.SM_Arrow'"));
	if (StaticMeshAsset.Succeeded())
	{
		ArrowMesh->SetStaticMesh(StaticMeshAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("ParticleSystem'/Game/PirateIsland/Include/Materials/Weapon/P_TrailArrow.P_TrailArrow'"));  
	if (ParticleAsset.Succeeded())
	{
		TrailParticleEffect = ParticleAsset.Object;
		ParticleSystemComponent->SetTemplate(TrailParticleEffect); 
	}
	

	Capsule->BodyInstance.bNotifyRigidBodyCollision = true;   
	Capsule->SetCollisionProfileName("BlockAll");
	bIsShooting = false;
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
	if (ParticleSystemComponent)
	{
		ParticleSystemComponent->EndTrails();

	}

	if (OnEndPlay.IsBound())
		OnEndPlay.Broadcast(this);
}

void ACArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(bIsShooting)
		RotateArrow();
	//CDebug::Print("Projectile Speed: ", Projectile->Velocity);
	FVector CapsuleLocation = Capsule->GetComponentLocation();
	FRotator CapsuleRotation = Capsule->GetComponentRotation();
	float CapsuleHalfHeight = Capsule->GetScaledCapsuleHalfHeight();
	float CapsuleRadius = Capsule->GetScaledCapsuleRadius();

	
	//DrawDebugCapsule(
	//	GetWorld(),
	//	CapsuleLocation,
	//	CapsuleHalfHeight,
	//	CapsuleRadius,
	//	CapsuleRotation.Quaternion(),
	//	FColor::Green,  
	//	false,          
	//	-1.0f,          
	//	0,            
	//	2.0f            
	//);

}

void ACArrow::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	bIsShooting = false;
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
	ForwardVector = InFoward;
	Projectile->Velocity = ForwardVector * Projectile->InitialSpeed;
	RotateArrow();
	if (ParticleSystemComponent)
	{
		ParticleSystemComponent->BeginTrails("Trail1", "Trail2", ETrailWidthMode::ETrailWidthMode_FromCentre, 1);
		
	}
	Projectile->Activate();
	bIsShooting = true;
	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

}



void ACArrow::RotateArrow()
{
	FRotator NewRotation = FRotationMatrix::MakeFromX(Projectile->Velocity).Rotator(); //look at 
	Capsule->SetWorldRotation(NewRotation);

}

