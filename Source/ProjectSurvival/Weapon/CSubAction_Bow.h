// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/CSubAction.h"
#include  "Components/TimelineComponent.h"
#include "CSubAction_Bow.generated.h"

USTRUCT(BlueprintType)
struct FAimData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float TargetArmLength = 100.0f;
	UPROPERTY(EditAnywhere) 
		FVector SocketOffset = FVector(0, 30, 10);
	UPROPERTY(EditAnywhere)
		bool bEnableCameraLag = false;  
	UPROPERTY(EditAnywhere) 
		FVector CameraLocation = FVector(0, 0, 0);
};


UCLASS()
class PROJECTSURVIVAL_API UCSubAction_Bow : public UCSubAction
{
	GENERATED_BODY()

public:
	UCSubAction_Bow();
public:  
	void  BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCEquipment* InEquipment, class UCDoAction* InDoAction) override;
	void  Tick_Implementation(float InDeltaTime) override;
public: 
	void Pressed()    override;
	void Released()   override;

private:
	class USpringArmComponent* SpringArm;
	class UCameraComponent* Camera;
	class UCBowAnimInstance* AnimInstance_Bow;
private:
	UPROPERTY(EditAnywhere, Category = "Aiming")
		class UCurveVector* Curve;   
	UPROPERTY(EditAnywhere, Category = "Aiming")
		FAimData  AimData;
	UPROPERTY(EditAnywhere, Category = "Aiming")
		float  AimingSpeed = 200.0f; 

private: 
	UFUNCTION()
		void OnAiming(FVector Output);
private: 
	FTimeline Timeline;
	class UCDoAction_Bow* BowDoAction;
private:
	FAimData  OriginData; 
	float* Bend = nullptr;
	
};
