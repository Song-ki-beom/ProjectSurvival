// Fill out your copyright notice in the Description page of Project Settings.


#include "Build/CBuildStructure.h"

// Sets default values
ACBuildStructure::ACBuildStructure()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACBuildStructure::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACBuildStructure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

