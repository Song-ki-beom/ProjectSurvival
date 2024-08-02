// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CSurvivorController.h"

ACSurvivorController::ACSurvivorController()
{

}

void ACSurvivorController::BeginPlay()
{
	SetInputMode(FInputModeGameOnly());
}

