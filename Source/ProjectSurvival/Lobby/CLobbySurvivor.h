#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CLobbySurvivor.generated.h"

/**
* SetSurvivorName(const FText& InText) - CCustomizeWidget���� ȣ���, ������ ��� ���� ����, Ŭ���� ��� ���� ��û
* PerformSetSurvivorName(const FText& InText) - ���ø����̼� ���� ����, ��ġ ����, ���� ������Ʈ ȣ��
* RequestSetSurvivorName(const FText& InText) - Ŭ�󿡼� ȣ���ؼ� ������ ��û, �������� RequestSetSurvivorName_Implementation ã�� ����
* OnRep_ReplicatedSurvivorName() - ReplicatedSurvivorName ������ �ٲ� ȣ��Ǵ� �Լ�
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