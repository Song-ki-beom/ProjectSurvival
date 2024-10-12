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

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    UFUNCTION()
        void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
            UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


private:  
    UPROPERTY(VisibleAnywhere)
        class UCapsuleComponent* Capsule;
    UPROPERTY(VisibleAnywhere)
        class UProjectileMovementComponent* Projectile;

private:
    UPROPERTY(EditDefaultsOnly, Category = "LifeSpan")
        float LifeSpanAfterCollision = 1.0f;

public: 
    FProjectileHit       OnHit;
    FProjectileEndPlay   OnEndPlay;

public:
    void  Shoot(const FVector& InFoward);


private:
    TArray<AActor*> Ignores;

public:
    FORCEINLINE void AddIgnoreActor(AActor* InActor) { Ignores.Add(InActor); }

};
