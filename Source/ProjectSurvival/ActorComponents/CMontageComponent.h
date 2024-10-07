// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Components/ActorComponent.h"
#include "ActorComponents/CStateComponent.h"
#include "CMontageComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMontageFinalEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMontageInterrupted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayMontageNotifyBegin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayMontageNotifyEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFoodMontageNotifyEnd);


USTRUCT()
struct FMontagesData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		EStateType Type;
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;
	UPROPERTY(EditAnywhere)
		float PlayRate = 1.0f;
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSURVIVAL_API UCMontageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCMontageComponent();

protected:
	virtual void BeginPlay() override;


public:
	UFUNCTION()
		void  Montage_Play(UAnimMontage* InMontage, float InPlayRate);
	UFUNCTION(NetMulticast, Reliable)
		void BroadcastMontage_Play(UAnimMontage* InMontage, float InPlayRate);

	UFUNCTION()
		void  Montage_Play_Section(UAnimMontage* InMontage, FName SectionName);

	UFUNCTION()
		void OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	UFUNCTION()
		void OnMontageNotifyEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	UFUNCTION()
		void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
		void BindFoodMontageEnded();
	UFUNCTION()
		void OnFoodMontageEnded(UAnimMontage* Montage, bool bInterrupted);


public:
	FOnMontageFinalEnded OnMontageFinalEnded;
	FOnMontageInterrupted OnMontageInterrupted;
	FOnPlayMontageNotifyBegin OnPlayMontageNotifyBegin;
	FOnPlayMontageNotifyEnd OnPlayMontageNotifyEnd;
	FOnFoodMontageNotifyEnd OnFoodMontageNotifyEnd;

private:
	class ACharacter* OwnerCharacter;     
	float MontageDelay = 0.0f;

public:
	FORCEINLINE float GetMontageDelay() { return MontageDelay; }
};
