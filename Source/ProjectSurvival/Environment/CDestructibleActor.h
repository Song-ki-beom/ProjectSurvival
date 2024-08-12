// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#pragma warning(push)
#pragma warning(disable : 4996)
#pragma warning(disable : 4706)

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructibleMesh.h"
#include "CDestructibleActor.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACDestructibleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACDestructibleActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


public:
	class UDestructibleComponent* GetDestructibleComponent();
	void SetDestructibleMesh(UDestructibleMesh* InDestructibleMesh, FTransform  InstanceTransform);



	
	
private:
	UPROPERTY(VisibleAnywhere)
		class UDestructibleComponent* DestructibleComponent;

};
