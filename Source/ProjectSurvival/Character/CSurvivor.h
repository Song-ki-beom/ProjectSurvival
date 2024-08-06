// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#pragma warning(push)
#pragma warning(disable : 4996)

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "DestructibleComponent.h"
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

private:
	//Move
	void OnMoveForward(float InAxisValue);
	void OnMoveRight(float InAxisValue);
	void OnHorizontalLook(float InAxisValue);
	void OnVerticalLook(float InAxisValue);

public:
	//Slash
	void Slash();

private:
	//Name  
	void PerformSetSurvivorName(const FText& InText);
	UFUNCTION(Server, Reliable, WithValidation)
		void RequestSetSurvivorName(const FText& InText);
	void UpdateSurvivorNameWidget();

	//Slash 
	void SlashBoxTrace();
	bool CheckIsFoliageInstance(const FHitResult& Hit);
	void SwitchFoligeToDestructible(FString* hitIndex);
	void DestroyDestructible(class UDestructibleComponent* DestructibleComponent);
	
private:
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
		TSubclassOf<class UUserWidget> SurvivorNameClass;
	UPROPERTY(VisibleAnywhere)
		class UWidgetComponent* SurvivorNameWidgetComponent;
	
private:
	//Slash
	UPROPERTY(EditAnywhere)
	float TraceDistance = 50.0f;
	UPROPERTY(EditAnywhere, Category = "Slash")
		UDataTable* DestructibleDataTable;
	FTransform SpawnTransform;
//	class UInstancedStaticMeshComponent* InstancedMesh;
	int32 InstanceIndex=NO_INDEX;
	//class UDestructibleMesh* DestructibleMesh;


	//Name 
	UPROPERTY(ReplicatedUsing = OnRep_ReplicatedSurvivorName)
		FText ReplicatedSurvivorName;
	UFUNCTION()
		void OnRep_ReplicatedSurvivorName();
};

