// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#pragma warning(push)
#pragma warning(disable : 4996)

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "Struct/CWeaponStructures.h"
#include "CustomDataType/BuildStructureDataType.h"
#include "Perception/AISightTargetInterface.h"
#include "GenericTeamAgentInterface.h"
#include "Components/TimeLineComponent.h"
#include "CSurvivor.generated.h"
#define NO_INDEX -1



UCLASS()
class PROJECTSURVIVAL_API ACSurvivor : public ACharacter , public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ACSurvivor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	//Weapon
	void DoAction();
	void SubActionPressed();
	void SubActionReleased();
	void HoldAxe();

	//Interact
	void StartInteract();
	void FinishInteract();

	//Build
	void Build();
	void CancleBuild();

	//Mouse Event 
	void HandleMouseWheelUp();
	void HandleMouseWheelDown();

	class UCWeaponComponent* GetWeaponComponent() { return WeaponComponent; }
	class UCHarvestComponent* GetHarvestComponent() { return HarvestComponent; }
	class UCBuildComponent* GetBuildComponent() { return BuildComponent; }
	class UCMovingComponent* GetMovingComponent() { return MovingComponent; }
	class UCInventoryComponent* GetInventoryComponent() const {
		return InventoryComponent;
	}
	class UCInteractionComponent* GetInteractionComponent() const {
		return InteractionComponent;
	}
	class UCMontageComponent* GetMontageComponent() { return MontageComponent; }
	class UCStateComponent* GetStateComponent() { return StateComponent; }

	void SelectStructure(ESelectedStructure InKey, TSubclassOf< ACStructure> InClass, EBuildStructureElement InElement, FName InItemID);


	//Customize
	USkeletalMeshComponent* GetHeadMeshComponent() { return Head; }
	USkeletalMeshComponent* GetPantsMeshComponent() { return Pants; }
	USkeletalMeshComponent* GetBootsMeshComponent() { return Boots; }
	USkeletalMeshComponent* GetBodyMeshComponent() { return Body; }
	USkeletalMeshComponent* GetHandsMeshComponent() { return Hands; }
	USkeletalMeshComponent* GetAccessoryMeshComponent() { return Accessory; }


	//Damage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;
	
	//Special Action
	virtual void PerformDoSpecialAction(ESpecialState SpecialState);

	UFUNCTION(NetMulticast, Reliable)
		virtual void BroadcastDoSpecialAction(ESpecialState SpecialState);


	// 팀 ID 설정 및 반환 함수
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; };
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamId) override;


	
	UFUNCTION(Server, Reliable)
		void RequestDoSpecialAction(ESpecialState SpecialState);

	UFUNCTION()
	void SpawnBear();

private:
	//Name  
	void PerformSetSurvivorName(const FText& InText);
	UFUNCTION(Server, Reliable, WithValidation)
		void RequestSetSurvivorName(const FText& InText);
	void UpdateSurvivorNameWidget();

	//Damage
	void ApplyHitData();
	UFUNCTION(NetMulticast, Reliable)
		void BroadCastApplyHitData(FDamageData InDamageData);

	void Die();

	void RemoveCharacter();

	void SetSurvivorNameVisibility();

	UFUNCTION(NetMulticast, Reliable)
		void BroadcastRemoveSurvivor();

	UFUNCTION(Server, Reliable)
		void RequestRemoveSurvivor();

	UFUNCTION(NetMulticast, Reliable)
		void BroadcastHidePlayerLocation(uint32 InNetworkGUIDValue);

	UFUNCTION(Server, Reliable)
		void RequestHidePlayerLocation(uint32 InNetworkGUIDValue);

	UFUNCTION()
		void SetWorldMapOpacity(float Value);

protected:
	UPROPERTY()
		FGenericTeamId TeamId; // 팀 ID 저장 변수


private:
	float MaxDistanceForNameVisibility = 2000.0f;

	UPROPERTY()
		class ACSurvivor* PersonalSurvivor;
	UPROPERTY()
		class UCGameInstance* GameInstance;
	UPROPERTY()
		class UNetDriver* NetDriver;

	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY()
		class USkeletalMeshComponent* Head;
	UPROPERTY()
		class USkeletalMeshComponent* Pants;
	UPROPERTY()
		class USkeletalMeshComponent* Boots;
	UPROPERTY()
		class USkeletalMeshComponent* Accessory;
	UPROPERTY()
		class USkeletalMeshComponent* Body;
	UPROPERTY()
		class USkeletalMeshComponent* Hands;


	UPROPERTY(VisibleAnywhere)
		TSubclassOf<class UUserWidget> SurvivorNameClass;
	UPROPERTY(VisibleAnywhere)
		class UWidgetComponent* SurvivorNameWidgetComponent;

	//Name 
	UPROPERTY(ReplicatedUsing = OnRep_ReplicatedSurvivorName)
		FText ReplicatedSurvivorName;
	UFUNCTION()
		void OnRep_ReplicatedSurvivorName();

	//Customize
	UPROPERTY(VisibleAnywhere)
		class UCCustomizeComponent* CustomizeComponent;
	UPROPERTY(VisibleAnywhere)
		class UCWeaponComponent* WeaponComponent;
	UPROPERTY(VisibleAnywhere)
		class UCHarvestComponent* HarvestComponent;
	UPROPERTY(VisibleAnywhere)
		class UCBuildComponent* BuildComponent;
	UPROPERTY(VisibleAnywhere)
		class UCMovingComponent* MovingComponent;
	UPROPERTY(VisibleAnywhere)
		class UCStatusComponent* StatusComponent;
	UPROPERTY(VisibleAnywhere)
		class UCStateComponent* StateComponent;
	UPROPERTY(VisibleAnywhere)
		class UCMontageComponent* MontageComponent;
	//UPROPERTY(VisibleAnywhere)
	//class UAIPerceptionStimuliSourceComponent* PerceptionStimuliSource;
	
	//Inventory
	UPROPERTY(VisibleAnywhere)
		class UCInteractionComponent* InteractionComponent;
	UPROPERTY(VisibleAnywhere ,Replicated)
		class UCInventoryComponent* InventoryComponent;

	//Damage
	UPROPERTY(EditAnywhere, Category = "HitData")
	FDamageData DamageData;
	FHitData* HitData;

	//Special Action
	UPROPERTY(EditAnywhere)
		TArray<FDoActionData> DoSpecialActionDatas;

	FTimeline WorldMapOpacityTimeline;

	UPROPERTY()
		float WorldMapOpacityTimelineDeltaTime = 0.0f;

	UPROPERTY()
		float WorldMapOpacityTimelineLength = 2.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Timeline")
		class UCurveFloat* WorldMapOpacityTimelineCurve;

public:
	UFUNCTION(Server, Reliable)
		void RequestMessage(const FText& InSurvivorNameText, const FText& InMessageText);

	UFUNCTION(NetMulticast, Reliable)
		void BroadcastMessage(const FText& InSurvivorNameText, const FText& InMessageText);

	UFUNCTION()
		void ReceiveMessage(const FText& InSurvivorNameText, const FText& InMessageText);

	UFUNCTION()
		void PerformAddMessage(const FText& InSurvivorNameText, const FText& InMessageText);

	UFUNCTION(NetMulticast, Reliable)
		void BroadcastSetName(const FText& InText, uint32 NetGUIDValue);

	UFUNCTION(Server, Reliable)
		void RequestSetName(const FText& InText, uint32 NetGUIDValue);

	UFUNCTION(NetMulticast, Reliable)
		void BroadcastSetLocation(float LocationX, float LocationY, float RotationZ, uint32 NetGUIDValue);

	UFUNCTION(Server, Reliable)
		void RequestSetLocation(float LocationX, float LocationY, float RotationZ, uint32 NetGUIDValue);

	UFUNCTION(NetMulticast, Reliable)
		void BroadcastShowPlayerLocation(uint32 InNetworkGUIDValue);

	UFUNCTION(Server, Reliable)
		void RequestShowPlayerLocation(uint32 InNetworkGUIDValue);

	UFUNCTION(NetMulticast, Reliable)
		void BroadcastRespawnSurvivor(FVector InLocation);

	UFUNCTION(Server, Reliable)
		void RequestRespawnSurvivor(FVector InLocation);

	UPROPERTY()
		class UCChattingBox* ChattingBox;
};

