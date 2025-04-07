// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CTestActorForRep.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACTestActorForRep : public AActor
{
	GENERATED_BODY()
	
public:	
	ACTestActorForRep();
	UFUNCTION(Server, Reliable )
	void RequestTestRPC();
	UFUNCTION(Server, Reliable)
	void OnServerSetOwnwer(class AController* survivorController);



protected:
	virtual void BeginPlay() override;

private:

	UFUNCTION(NetMulticast, Reliable)
	void BroadcastTestRPC();
	UFUNCTION(Client, Reliable)
	void ClientTestRPC();


public:	
	virtual void Tick(float DeltaTime) override;



protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
		UStaticMeshComponent* PickupMesh;

private:
	FGuid MyGUID;


};
