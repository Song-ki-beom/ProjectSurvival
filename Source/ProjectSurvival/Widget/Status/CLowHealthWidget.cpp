// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Status/CLowHealthWidget.h"
#include "ActorComponents/CStatusComponent.h"
#include "GameFramework/Character.h"
#include "Components/ProgressBar.h"

void UCLowHealthWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();


}

void UCLowHealthWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ACharacter* OwnerCharacter = GetOwningPlayer()->GetCharacter();
	if (OwnerCharacter == nullptr) return;
	StatusComponent = Cast<UCStatusComponent>(OwnerCharacter->GetComponentByClass(UCStatusComponent::StaticClass()));
	StatusComponent->OnLowHealthDetected.AddUObject(this, &UCLowHealthWidget::ShowLowHealthEffect);

}

void UCLowHealthWidget::ShowLowHealthEffect()
{
    if (LowHealthAnim)
    {
      
        this->SetVisibility(ESlateVisibility::Visible);

      
        PlayAnimation(LowHealthAnim);

        GetWorld()->GetTimerManager().SetTimer(LowHealthTimerHandle, this, &UCLowHealthWidget::HideLowHealthEffect, 0.7f, false);
    }
}

void UCLowHealthWidget::HideLowHealthEffect()
{
    SetVisibility(ESlateVisibility::Hidden);
}