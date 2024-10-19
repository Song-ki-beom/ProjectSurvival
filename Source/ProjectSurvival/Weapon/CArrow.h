// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CArrow.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FProjectileHit, class AActor*, InCauser, class ACharacter*, InOtherCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProjectileEndPlay, class ACArrow*, InDestroyer);


UCLASS()
class PROJECTSURVIVAL_API ACArrow : public AActor
{
    GENERATED_BODY()

public:
    ACArrow();
public:
    void Shoot(const FVector& InFoward);

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void Tick(float DeltaTime) override;
private:
    UFUNCTION()
        void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
            UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
    void RotateArrow();


public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
        UParticleSystemComponent* ParticleSystemComponent;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
        UParticleSystem* TrailParticleEffect;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
        class UStaticMeshComponent* ArrowMesh;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
        UStaticMesh* MeshAsset;


public:
    FProjectileHit       OnHit;
    FProjectileEndPlay   OnEndPlay;

private:  

    UPROPERTY(VisibleAnywhere)
        class UCapsuleComponent* Capsule;
    UPROPERTY(VisibleAnywhere)
        class UProjectileMovementComponent* Projectile;
    UPROPERTY()
        class ACharacter* OwnerCharacter;
    UPROPERTY(EditDefaultsOnly, Category = "LifeSpan")
        float LifeSpanAfterCollision = 1.0f;

private:
    UPROPERTY()
        TArray<AActor*> Ignores;
    bool bIsShooting;
    FVector ForwardVector;
public:
    FORCEINLINE void AddIgnoreActor(AActor* InActor) { Ignores.Add(InActor); }

};
