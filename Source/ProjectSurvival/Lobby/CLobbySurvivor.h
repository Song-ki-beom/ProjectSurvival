#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CLobbySurvivor.generated.h"

/**
* SetSurvivorName(const FText& InText) - CCustomizeWidget에서 호출됨, 서버인 경우 직접 설정, 클라인 경우 설정 요청
* PerformSetSurvivorName(const FText& InText) - 리플리케이션 변수 변경, 위치 변경, 위젯 업데이트 호출
* RequestSetSurvivorName(const FText& InText) - 클라에서 호출해서 서버에 요청, 서버에서 RequestSetSurvivorName_Implementation 찾아 실행
* OnRep_ReplicatedSurvivorName() - ReplicatedSurvivorName 변수가 바뀔때 호출되는 함수
*/

UCLASS()
class PROJECTSURVIVAL_API ACLobbySurvivor : public ACharacter
{
	GENERATED_BODY()

public:
	ACLobbySurvivor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void SetLocalValue();
	void SetSurvivorName(const FText& InText);
	void PerformSetSurvivorName(const FText& InText);
	UFUNCTION(Server, Reliable, WithValidation)
		void RequestSetSurvivorName(const FText& InText);
	void UpdateSurvivorNameWidget();

	UFUNCTION(Server, Reliable, WithValidation)
		void RequestReady();

	UFUNCTION(NetMulticast, Reliable)
		void BroadcastSetText();

private:
	void OnMoveForward(float InAxisValue);
	void OnMoveRight(float InAxisValue);

private:
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
		TSubclassOf<class UUserWidget> SurvivorNameClass;
	UPROPERTY(VisibleAnywhere)
		class UWidgetComponent* SurvivorNameWidgetComponent;

	FVector OriginalSpringArmLocation;

	UPROPERTY(ReplicatedUsing = OnRep_ReplicatedSurvivorName)
		FText ReplicatedSurvivorName;
	UFUNCTION()
		void OnRep_ReplicatedSurvivorName();
};
