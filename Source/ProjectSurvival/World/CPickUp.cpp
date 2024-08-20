// Fill out your copyright notice in the Description page of Project Settings.


#include "World/CPickUp.h"

// Sets default values
ACPickUp::ACPickUp()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACPickUp::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACPickUp::Interact(UCInteractionComponent* InteractComponent)
{

}

void ACPickUp::TakePickup(const ACSurvivor* Taker)
{

}

// Called every frame
void ACPickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPickUp::BeginFocus()
{
}

void ACPickUp::EndFocus()
{
}


void ACPickUp::InitializePickup(const TSubclassOf<class UItemBase> BaseClass, const int32 InQuantity)
{

}

void ACPickUp::InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity)
{

}


void ACPickUp::BeginInteract()
{
}

void ACPickUp::EndInteract()
{
}
