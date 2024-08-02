// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/CDifficultyWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

bool UCDifficultyWidget::Initialize()
{
	bool Sucess = Super::Initialize();

	if (!Sucess)
		return false;

	if (!IsValid(EasyButton)) { UE_LOG(LogTemp, Warning, TEXT("EasyButton is invalid")); return false; }
	EasyButton->OnClicked.AddDynamic(this, &UCDifficultyWidget::OnEasy);

	if (!IsValid(NormalButton)) { UE_LOG(LogTemp, Warning, TEXT("NormalButton is invalid")); return false; }
	NormalButton->OnClicked.AddDynamic(this, &UCDifficultyWidget::OnNormal);

	if (!IsValid(HardButton)) { UE_LOG(LogTemp, Warning, TEXT("HardButton is invalid")); return false; }
	HardButton->OnClicked.AddDynamic(this, &UCDifficultyWidget::OnHard);

	if (!IsValid(ExtremeButton)) { UE_LOG(LogTemp, Warning, TEXT("ExtremeButton is invalid")); return false; }
	ExtremeButton->OnClicked.AddDynamic(this, &UCDifficultyWidget::OnExtreme);

	if (!IsValid(StartButton)) { UE_LOG(LogTemp, Warning, TEXT("StartButton is invalid")); return false; }
	StartButton->OnClicked.AddDynamic(this, &UCDifficultyWidget::OnStart);

	return true;
}

void UCDifficultyWidget::OnEasy()
{

}

void UCDifficultyWidget::OnNormal()
{

}

void UCDifficultyWidget::OnHard()
{

}

void UCDifficultyWidget::OnExtreme()
{

}

void UCDifficultyWidget::OnStart()
{

}