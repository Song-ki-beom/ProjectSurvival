// Fill out your copyright notice in the Description page of Project Settings.


#include "CSurvivorController.h"
#include "CSurvivor.h"
#include "Engine/Engine.h"
#include "InputCoreTypes.h"
#include "GameFramework/Actor.h"

ACSurvivorController::ACSurvivorController()
{

}

void ACSurvivorController::BeginPlay()
{
	this->SetInputMode(FInputModeGameOnly());
}

void ACSurvivorController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		/*InputComponent->BindKey(EKeys::R, IE_Pressed , this , &ACSurvivorController::Slash);*/
		InputComponent->BindKey(EKeys::T, IE_Pressed, this, &ACSurvivorController::HoldAxe);
		InputComponent->BindKey(EKeys::LeftMouseButton, IE_Pressed, this, &ACSurvivorController::DoAction);


	}
}

void ACSurvivorController::DoAction()
{
	ACSurvivor* controlledCharacter = Cast<ACSurvivor>(GetCharacter());

	if (controlledCharacter)
	{
		controlledCharacter->DoAction();
	}

}

void ACSurvivorController::HoldAxe()
{
	ACSurvivor* controlledCharacter = Cast<ACSurvivor>(GetCharacter());

	if (controlledCharacter)
	{
		controlledCharacter->HoldAxe();
	}

}


