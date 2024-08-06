// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Lobby/CLobbySurvivor.h"
#include "CCustomizeComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTSURVIVAL_API UCCustomizeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCCustomizeComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void DoSkeletalMeshReplicate();

	// 머리
	void PerformSetHeadMesh(FName InHeadName, FName InHeadColorName, FName InPantsName, FName InBootsName, FName InSingleName, FName InSkinColorName);
	UFUNCTION(Server, Reliable, WithValidation)
		void RequestSetHeadMesh(FName InHeadName, FName InHeadColorName, FName InPantsName, FName InBootsName, FName InSingleName, FName InSkinColorName);
	UFUNCTION()
		void OnRep_ReplicatedHeadName();
		void UpdateHeadMesh();
	UFUNCTION()
		void OnRep_ReplicatedHeadColorName();
		void UpdateHeadColor();
	UFUNCTION()
		void OnRep_ReplicatedPantsName();
		void UpdatePantsMesh();
	UFUNCTION()
		void OnRep_ReplicatedBootsName();
		void UpdateBootsMesh();
	UFUNCTION()
		void OnRep_ReplicatedSingleName();
		void UpdateSingleMesh();
		UFUNCTION()
		void OnRep_ReplicatedSkinColorName();
		void UpdateSkinColor();
private:
	AActor* ComponentOwner;
	class ACSurvivor* Survivor;
	TArray<FName> CustomizeRowNames;

	// 커스터마이징 관련
	class UDataTable* CustomizeHeadData;
	class UDataTable* CustomizeHeadColorData;
	class UDataTable* CustomizePantsData;
	class UDataTable* CustomizePantsColorData;
	class UDataTable* CustomizeBootsData;
	class UDataTable* CustomizeSkinColorData;
	class UDataTable* CustomizeSingleData;

	UPROPERTY(ReplicatedUsing = OnRep_ReplicatedHeadName)
		FName ReplicatedHeadName;
	UPROPERTY(ReplicatedUsing = OnRep_ReplicatedHeadColorName)
		FName ReplicatedHeadColorName;
	UPROPERTY(ReplicatedUsing = OnRep_ReplicatedPantsName)
		FName ReplicatedPantsName;
	UPROPERTY(ReplicatedUsing = OnRep_ReplicatedBootsName)
		FName ReplicatedBootsName;
	UPROPERTY(ReplicatedUsing = OnRep_ReplicatedSingleName)
		FName ReplicatedSingleName;
	UPROPERTY(ReplicatedUsing = OnRep_ReplicatedSkinColorName)
		FName ReplicatedSkinColorName;
};
