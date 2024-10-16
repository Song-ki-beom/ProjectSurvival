// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CrossHair/CCrossHairWidget.h"
#include "GameFramework/Character.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"


void UCCrossHairWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();


}

void UCCrossHairWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ACharacter* OwnerCharacter = GetOwningPlayer()->GetCharacter();
	if (OwnerCharacter == nullptr) return;
	
	//CrossHair->SetBrushFromTexture();


}





