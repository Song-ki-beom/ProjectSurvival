#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CLobbySurvivorController.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACLobbySurvivorController : public APlayerController
{
	GENERATED_BODY()

public:
	ACLobbySurvivorController();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
		void LoadCustomizeWidget();
	UFUNCTION()
		void LoadWaitingWidget();

private:
	UPROPERTY()
		TSubclassOf<class UUserWidget> CustomizeClass;
	UPROPERTY()
		class UCCustomizeWidget* CustomizeWidget;
	UPROPERTY()
		TSubclassOf<class UUserWidget> WaitingClass;
	UPROPERTY()
		class UCWaitingWidget* WaitingWidget;

	class ACLobbySurvivor* LobbySurvivor;
};
