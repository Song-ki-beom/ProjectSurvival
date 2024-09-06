// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/InteractionInterface.h"
#include "CInteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSURVIVAL_API UCInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCInteractionComponent();



public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	bool IsInteracting() const;
	void DoInteract();
	void FinishInteract();
	void UpdateInteractionWidget() const;
	void ExtraOptionButtonUp();
	void ExtraOptionButtonDown();
protected:
	virtual void BeginPlay() override;
	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	void BeginInteract();
	void EndInteract();
	void Interact();
	void ToggleHiddenMenu();
	void ShowHiddenMenu();
	void HideHiddenMenu();
private:
	UFUNCTION(Server, Reliable)
		void RequestDestroy();
	UFUNCTION(Server, Reliable)
		void RequestUpdatePartialAdded(int32 InQuantity);
	UFUNCTION(NetMulticast, Reliable)
		void BroadcastUpdatePartialAdded(int32 InQuantity);




protected:
	UPROPERTY(VisibleAnywhere, Category = "Interaction")
		TScriptInterface<IInteractionInterface> TargetInteractable;

	float InteractionCheckFrequency;
	float InteractionCheckDistance;

	FInteractionData InteractionData;
	FTimerHandle LongPressTimerHandle;

private:
	class ACharacter* OwnerCharacter;
	UPROPERTY()
	class ACMainHUD* HUD;
	bool bIsLongPress;
	bool bIsExtraOptionOn;
};
