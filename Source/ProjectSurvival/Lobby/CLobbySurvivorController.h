#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CLobbySurvivorController.generated.h"

/**
* 플레이어 개별 위젯 처리는 컨트롤러에서 할 것
*/

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
	void GetCustomizeInfo();

	UFUNCTION()
		void LoadWaitingWidget();
	UFUNCTION()
		void SetVisibleWaitingWidget();

	void UpdateDifficultyWidget(int InIndex);

	void UpdateClientDifficulty();

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

