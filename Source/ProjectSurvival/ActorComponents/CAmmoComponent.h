// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAmmoComponent.generated.h"

USTRUCT()
struct FArrow
{
	GENERATED_BODY()
	UPROPERTY()
	bool bShooting = false;
	UPROPERTY()
	class ACArrow* Arrow = nullptr;
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSURVIVAL_API UCAmmoComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCAmmoComponent();

protected:
	virtual void BeginPlay() override;

public:
	void CreateArrow();
	void ShootArrow(FVector Start, FVector End);
	void PerformShootArrow(FVector Start, FVector End);
	int32 GetAttachedArrow();
	class ACArrow* GetArrow(int32 ArrowIndex);

	void DestroyArrows();
public:
	UFUNCTION() void OnArrowHit(class AActor* InCauser, class ACharacter* InOtherCharacter);
	UFUNCTION() void OnArrowEndPlay(class ACArrow* InDestroyer);
	UFUNCTION() void ShowAttachedArrow();
	UFUNCTION() void HideAttachedArrow();


private:
	UFUNCTION(NetMulticast, Reliable)
	void BroadcastShowAttachedArrow();
	UFUNCTION(Server, Reliable)
	void RequestShowAttachedArrow();
	UFUNCTION(NetMulticast, Reliable)
	void BroadcastHideAttachedArrow();
	UFUNCTION(Server, Reliable)
	void RequestHideAttachedArrow();
	UFUNCTION(NetMulticast, Reliable)
	void BroadcastRemoveArrow(int32 Arrowidx);
	UFUNCTION(NetMulticast, Reliable)
	void BroadcastShootArrow(ACArrow* TargetArrow, FVector Direction);
	UFUNCTION(Server, Reliable)
	void RequestShootArrow(FVector Start, FVector End);


	UFUNCTION(NetMulticast, Reliable)
	void BroadcastInitializeArrow(class ACArrow* TargetArrow);
	UFUNCTION(NetMulticast, Reliable)
	void BroadcastAddArrow(class ACArrow* TargetArrow);


public:
	UPROPERTY(EditDefaultsOnly, Category = "Arrow")
		TSubclassOf<class ACArrow> ArrowClass;
	UPROPERTY()
		FArrow TempArrow;
private:
	UPROPERTY()
		class ACharacter* OwnerCharacter;
	UPROPERTY()
		class UCStateComponent* StateComponent;

	TArray<FArrow> Arrows;
	bool bArrowFullyCreated;
	
public:
	FORCEINLINE bool GetIsArrowFullyCreated() {
		return bArrowFullyCreated;
	}


};
