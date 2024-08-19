// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#pragma warning(push)
#pragma warning(disable : 4996)

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "CustomDataType/BuildStructureDataType.h"
#include "CSurvivor.generated.h"
#define NO_INDEX -1



UCLASS()
class PROJECTSURVIVAL_API ACSurvivor : public ACharacter 
{
	GENERATED_BODY()

public:
	ACSurvivor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

//private:
//	//Move
//	void OnMoveForward(float InAxisValue);
//	void OnMoveRight(float InAxisValue);
//	void OnHorizontalLook(float InAxisValue);
//	void OnVerticalLook(float InAxisValue);
public:
	//Weapon
	void DoAction();
	void SubAction();
	void HoldAxe();

	//Interact
	void StartInteract();
	void FinishInteract();


	//Build
	void Build();
	void CancleBuild();

	class UCWeaponComponent* GetWeaponComponent() { return WeaponComponent; }
	class UCHarvestComponent* GetHarvestComponent() { return HarvestComponent; }
	class UCBuildComponent* GetBuildComponent() { return BuildComponent; }
	class UCMovingComponent* GetMovingComponent() { return MovingComponent; }

	void SelectStructure(ESelectedStructure InKey, TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement);


	//Customize
	USkeletalMeshComponent* GetHeadMeshComponent() { return Head; }
	USkeletalMeshComponent* GetPantsMeshComponent() { return Pants; }
	USkeletalMeshComponent* GetBootsMeshComponent() { return Boots; }
	USkeletalMeshComponent* GetBodyMeshComponent() { return Body; }
	USkeletalMeshComponent* GetHandsMeshComponent() { return Hands; }
	USkeletalMeshComponent* GetAccessoryMeshComponent() { return Accessory; }



private:
	//Name  
	void PerformSetSurvivorName(const FText& InText);
	UFUNCTION(Server, Reliable, WithValidation)
		void RequestSetSurvivorName(const FText& InText);
	void UpdateSurvivorNameWidget();

	
	
private:
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
		class UCInteractionComponent* InteractionComponent;
};

