// Fill out your copyright notice in the Description page of Project Settings.


#include "CSurvivorController.h"
#include "CSurvivor.h"
#include "Engine/Engine.h"
#include "InputCoreTypes.h"
#include "GameFramework/Actor.h"

ACSurvivorController::ACSurvivorController()
{

}

void ACSurvivorController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		InputComponent->BindKey(EKeys::R, IE_Pressed , this , &ACSurvivorController::SlashTree);
	}
}

void ACSurvivorController::SlashTree()
{
	ACSurvivor* controlledCharacter = Cast<ACSurvivor>(GetCharacter());

	if (controlledCharacter)
	{
		controlledCharacter->SlashTree();
	}

}


