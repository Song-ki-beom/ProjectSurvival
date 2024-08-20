// Fill out your copyright notice in the Description page of Project Settings.


#include "World/CPickUp.h"

// Sets default values
ACPickUp::ACPickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACPickUp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

