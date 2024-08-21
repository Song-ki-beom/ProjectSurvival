// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/CInventoryComponent.h"

UCInventoryComponent::UCInventoryComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

}


void UCInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UCInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

